
const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const pty = require('node-pty');
const path = require('path');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

app.use(express.static('public'));

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

io.on('connection', (socket) => {
  console.log('User connected');
  
  const term = pty.spawn('./Chess.exe', [], {
    name: 'xterm-color',
    cols: 80,
    rows: 40,
    cwd: process.cwd(),
    env: process.env
  });

  term.on('data', (data) => {
    socket.emit('output', data);
  });

  socket.on('input', (data) => {
    term.write(data);
  });

  socket.on('disconnect', () => {
    term.kill();
    console.log('User disconnected');
  });
});

const PORT = process.env.PORT || 5000;
server.listen(PORT, '0.0.0.0', () => {
  console.log(`Server running on port ${PORT}`);
});
