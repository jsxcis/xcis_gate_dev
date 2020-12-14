R"(
<!DOCTYPE html>
<html>
    <head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
    <title>XCIS Gateway</title>
    <style>
         .mybody{
        background-color: #5D5D5D;
        color: #FFFFFF;
        font-family: Arial, Helvetica, sans-serif;
        font-size: 14px;
        }
        .buttonStyleNormal{
        height:30px;
        width:130px;
        border-radius: 2px;
        background-color:#9B9B9B;
        color:white;
        }
        .buttonStyleNormal:hover {
        background-color: #D5D5D5;
        }
        .buttonStyleNormalRed{
        height:30px;
        width:100px;
        border-radius: 2px;
        background-color:red;
        color:white;
        }
        .buttonStyleNormalRed:hover {
        background-color: #404040
        }
    </style>
    </head>
    <body class="mybody">
    <div id="header" style="overflow:hidden;">
    <h4 style="top:10px;left:970px;position:absolute;"><span id="datetime"></span></h4>
    <script>
    var dt = new Date();
    document.getElementById("datetime").innerHTML = dt.toLocaleString();
    </script>
    <h1 style="top:0px;left:0px;position:relative;">XCIS Gateway Console</h1>
    <hr style="width: 100%; height: 2px; background-color: #FFFFFF">
    </div>
    <div id="content" style="position:absolute;top:85px;bottom:10px;left:0px;right:0px;overflow:auto;">
        <a href="displayMain" style="top:10px;left:8px;position:absolute;"><button class="buttonStyleNormal">Sensors</button></a>
        <a href="displayAdmin" style="top:45px;left:8px;position:absolute;"><button class="buttonStyleNormal">Admin</button></a>
        <a href="displayStatus" style="top:80px;left:8px;position:absolute;"><button class="buttonStyleNormal">Status</button></a>
        <a href="displayMesh" style="top:115px;left:8px;position:absolute;"><button class="buttonStyleNormal">Mesh</button></a>
        <a href="logout" style="top:150px;left:8px;position:absolute;"><button class="buttonStyleNormal">Logout</button></a>
        <iframe name="SensorList" style="top:10px;left:150px;position:absolute;" src="listsensors" width="550" height="800" seamless="seamless" style="vertical-align: top"></iframe>
        <iframe name="SensorListHelp" style="top:10px;left:720px;position:absolute;" src="listsensorshelp" width="400" height="800" seamless="seamless" style="vertical-align: top"></iframe>
    </div>
    </body>
</html>
)"
