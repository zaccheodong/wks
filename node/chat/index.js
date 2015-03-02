var app = require('express')();
var http = require('http').Server(app);
var io   = require('socket.io')(http);

app.get('/',function(req,res){
    res.send('<h1>Welcom Realtime Server</h1>');
});

http.listen(3000,function(){
    console.log('listening on *:3000');
});


var onlineUsers ={};
var onlineCount= 0;

io.on('connection',function(socket){
    console.log('a user connected');
    
    socket.on('login',function(obj){
        console.log('login');
        
        socket.name = obj.userid;
        
        if(!onlineUsers.hasOwnProperty(obj.userid)){
            onlineUsers[obj.userid] = obj.username;
            onlineCount++;
        }
        
        io.emit('login',{onlineUsers:onlineUsers,onlineCount:onlineCount,user:obj});
        console.log(obj.username+'加入了聊天室');
    });
    
    
    socket.on('disconnect',function(){
        console.log('disconnect');
        
        if(!onlineUsers.hasOwnProperty(socket.name))
            console.log('can not find '+socket.name);
        
        var obj = {userid:socket.name,username:onlineUsers[socket.name]};
        delete onlineUsers[socket.name];
        
        onlineCount--;
        
        io.emit('logout',{onlineUsers:onlineUsers,onlineCount:onlineCount,user:obj});
        
        console.log(obj.username+'退出了聊天室');
    });
    
    
    socket.on('message',function(obj){
        console.log('message');
        
        io.emit('message',obj);
        console.log(obj.username+'说：'+obj.content);
    });
    
});