const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const pty = require('node-pty');
const path = require('path');
const fs = require('fs');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

app.use(express.static('public'));

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Function to check if chess executable exists and is executable
function checkChessExecutable() {
  const chessPath = path.join(__dirname, 'Chess');
  
  try {
    if (!fs.existsSync(chessPath)) {
      console.error(`Chess executable not found at: ${chessPath}`);
      return false;
    }
    
    const stats = fs.statSync(chessPath);
    if (!(stats.mode & parseInt('111', 8))) {
      console.error(`Chess executable is not executable. Run: chmod +x ${chessPath}`);
      return false;
    }
    
    console.log(`Chess executable found and is executable: ${chessPath}`);
    return true;
  } catch (error) {
    console.error(`Error checking chess executable: ${error.message}`);
    return false;
  }
}

io.on('connection', (socket) => {
  console.log('User connected');
  
  if (!checkChessExecutable()) {
    socket.emit('output', 'Error: Chess executable not found or not executable\r\n');
    socket.emit('output', 'Please ensure the Chess binary exists and has execute permissions\r\n');
    return;
  }
  
  let term;
  
  try {
    // Use absolute path to the chess executable
    const chessPath = path.join(__dirname, 'Chess');
    
    console.log(`Attempting to spawn: ${chessPath}`);
    
    term = pty.spawn(chessPath, [], {
      name: 'xterm-color',
      cols: 152, // Match your terminal size
      rows: 33,
      cwd: __dirname, // Use project directory as working directory
      env: {
        ...process.env,
        TERM: 'xterm-256color'
      }
    });

    term.on('data', (data) => {
      socket.emit('output', data);
    });

    term.on('exit', (code, signal) => {
      console.log(`Chess process exited with code ${code}, signal ${signal}`);
      socket.emit('output', `\r\nChess game ended (exit code: ${code})\r\n`);
    });

    term.on('error', (error) => {
      console.error('Terminal error:', error);
      socket.emit('output', `Terminal error: ${error.message}\r\n`);
    });

    socket.on('input', (data) => {
      if (term && !term.killed) {
        term.write(data);
      }
    });

    socket.on('resize', (data) => {
      if (term && !term.killed) {
        term.resize(data.cols, data.rows);
      }
    });

    socket.on('disconnect', () => {
      if (term && !term.killed) {
        term.kill();
      }
      console.log('User disconnected');
    });

  } catch (error) {
    console.error('Error spawning chess process:', error);
    socket.emit('output', `Error starting chess game: ${error.message}\r\n`);
  }
});

const PORT = process.env.PORT || 5000;

// Handle process termination gracefully
process.on('SIGTERM', () => {
  console.log('Received SIGTERM, shutting down gracefully');
  server.close(() => {
    process.exit(0);
  });
});

process.on('SIGINT', () => {
  console.log('Received SIGINT, shutting down gracefully');
  server.close(() => {
    process.exit(0);
  });
});

// Check chess executable before starting server
if (checkChessExecutable()) {
  server.listen(PORT, '0.0.0.0', () => {
    console.log(`Server running on port ${PORT}`);
    console.log(`Chess game ready to serve clients`);
  });
} else {
  console.error('Cannot start server: Chess executable issues');
  process.exit(1);
}