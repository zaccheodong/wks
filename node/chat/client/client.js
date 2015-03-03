(function(){
    var d =document;
    var w = window;
    var p = parseInt;
    var dd = d.documentElement;
    var db = d.body;
    var dc = d.compatMode == 'CSS1Compat';
    var dx = dc?dd:db;
    var ec = encodeURIComponent;
    
    w.CHAT={
        msgObj:d.getElementById("message"),
        screenheight:w.innerHeight?w.innerHeight:dx.clientHeight,
        username:null,
        userid:null,
        socket:null,
        
        scrollToBottom:function(){
        w.scrollTo(p,this.msgObj.clientHeight);
        },
        
        logout:function(){
            location.reload();
        },
        
        submit:function(){
            var content = d.getElementById("content").value;
            if(content != ''){
                var obj={
                    userid:this.userid,
                    username:this.usernamme,
                    content:content
                };
                this.socket.emit('message',obj);
                d.getElementById('content').value='';
            }
            
            return false;
        },
        
        genUid:function(){
            return new Date().getTime()+""+Math.floor(Math.random()*899+100);
        },
        udpateSysMsg:function(o,action){
            var onlineUsers = o.onlineUsers;
            var onlineCount = o.onlineCount;
            var user = o.user;
            
            var userhtml='';
            var separator='';
            for(key in onlineUsers){
                if(onlineUsers.hasOwnProperty(key)){
                    userhtml += separator +onlineUsers[key];
                    separator="、";
                }
            }
            
            d.getElementById("onlinecount").innerHTML="当前共有 "+onlineCount+"人在线，在线列表："+userhtml;
          
            var  html='';
            html += user.username;
            html += (action == "login")?"加入了聊天室":"退出了聊天室";
            var section = d.createElement('section');
            section.className="system J-mjrlinkWrap J-cutMsg";
            section.innerHTML=html;
            this.msgObj.appendChild(section);
            this.scrollToBottom();
            
        },
        
        usernameSubmit:function(){
                var username= d.getElementById("username").value;
                if(username !=""){
                    d.getElementById("username").value='';
                    d.getElementById("loginbox").style.display='none';
                    d.getElementById("chatbox").style.display='block';
                    this.init(username);
                }
                return false;
            },
        
        init:function(username){
            //客户端根据时间和随机数生成UID，这样使的聊天室用户名称可以重复
            this.userid = this.genUid;
            this.username = username;
            
            d.getElementById("showusername").innerHTML = this.username;
            this.msgObj.style.minHeight=(this.screenheight - db.clientHeight+this.msgObj.clientHeight)+"px";
            this.scrollToBottom();
            
            //链接websocket后端服务器
            this.socket = io.connect("ws://localhost:3000");
            
            //登录
            this.socket.emit("login",{userid:this.userid,username:this.username});
            
            //监听新用户登录
            this.socket.on("login",function(o){
                CHAT.udpateSysMsg(o,"login");
            });
            
            //监听用户注销
            this.socket.on("logout",function(o){
                CHAT.udpateSysMsg(o,"logout");
            });
            
            //监听消息发送
            this.socket.on("message",function(obj){
                var isme =(obj.userid == CHAT.userid)? true:false;
                var conentDiv=''+obj.content+'';
                
                var usernameDiv =" " +obj.username;
                var section = d.createElement("section");
                if(issame){
                    section.className="user";
                    section.innerHTML=contentDiv+usernameDiv
                }else{
                    section.className="service";
                    section.innerHTML=usernameDiv+contentDiv;
                }
                CHAT.msgObj.appendChild(section);
                CHAT.scrollToBottom();
                
                
            });
            
        },
        
    };
                
    
    d.getElementById("username").onkeydown=function(e){
                e = e||event;
                if(e.keyCode === 13){
                    CHAT.usernameSubmit();
                }
                };
                
    d.getElementById("content").onkeydown = function(e){
                e = e||event;
                if(e.keyCode === 13){
                    CHAT.submit();
                }
                
                };
})();