import QtQuick 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import QtWebSockets 1.0
import "qwebchannel.js" as WebChannel

Item {

    id: chatItem

    //LOOK WE MUST HANDLE THE MINIMZE or then we got error from web channel

    /*
    Timer
    {
        id: lateInitTimer
        interval: 2000

        onTriggered: {

                loginName = "U_" + userToken.getToken()

                console.log(loginName + " ;ogin name")

                //call the login method
                chatItem.channel.objects.aimserver.login(loginName, function(arg) {
                    //check the return value for success
                    if (arg === true) {
                        //loginUi.nameInUseError.visible = false;
                        //loginWindow.close();
                    } else {
                        //loginUi.nameInUseError.visible = true;
                    }
            });

        }
    }

    property string signedNickname : "notset"


    function pushSend(nickName)
    {
        if (msg.text.length) {
            //call the sendMessage method to send the message
            chatItem.channel.objects.aimserver.sendMessage(loginName,
                                                        msg.text, nickName);
        }
        msg.text = '';
    }



    Component.onCompleted:
    {
        lateInitTimer.start()
    }

    property var channel
    property string loginName: ""

    //property string nickName:  "noset";

    WebSocket {
        id: socket

        // the following three properties/functions are required to align the QML WebSocket API
        // with the HTML5 WebSocket API.
        property var send: function(arg) {
            sendTextMessage(arg);
        }

        onTextMessageReceived: {
            onmessage({data: message});
        }

        property var onmessage

        active: true
        url: "ws://192.168.1.169:12345"

        onStatusChanged: {
            switch (socket.status) {
            case WebSocket.Error:

                var textError = "Error: " + socket.errorString;
                console.log(textError)

                break;
            case WebSocket.Closed:

                var textClosed =  "Error: Socket at " + url + " closed.";
                console.log(textClosed)

                break;
            case WebSocket.Open:
                //open the webchannel with the socket as transport
                new WebChannel.QWebChannel(socket, function(ch) {
                    chatItem.channel = ch;

                    //connect to the changed signal of the userList property
                    ch.objects.aimserver.userListChanged.connect(function(args) {
                        tokensList.text = "";
                        ch.objects.aimserver.userList.forEach(function(user) {
                            tokensList.text += user + "\n";
                        });
                    });

                    //connect to the newMessage signal
                    ch.objects.aimserver.newMessage.connect(function(time, user, message) {
                        var line = "[" + time + "] " + user + ": " + message + '\n';
                        chatLog.text = chatLog.text + line;
                    });

                    //connect to the keep alive signal
                    ch.objects.aimserver.keepAlive.connect(function(args) {
                        if (loginName !== '')
                            //and call the keep alive response method as an answer
                            ch.objects.aimserver.keepAliveResponse(loginName);
                    });
                });

                break;
            }
        }
    }


    Text
    {
        id: chatLog
    }

    Text
    {
        id: tokensList
        x: parent.width/2
    }

    TextField
    {
        id: msg
        y: parent.height - parent.height/10

        onEditingFinished:
        {
            pushSend("nonick")
        }
    }
    */

}
