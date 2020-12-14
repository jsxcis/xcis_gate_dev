R"(
<!DOCTYPE html>
<html>
    <head>
        <title>XCIS Gateway</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <style>
        body {
        background-color: #000000;
        color: #FFFFFF;
        font-family: Arial, Helvetica, sans-serif;
        font-size: 12px;
        }
        a {
        color: #202ECC;
        }
        a:hover {
        color: #FFFFFF;
        }
      .v1 {
        border-left: 2px solid white;
        height:500px;
        }
    .buttonStyle
    {
    height:25px;
    width:100px;
    border-radius: 2px;
    background-color:#003399;
    color:white;
    }
    </style>
        <h1>Login: XCIS Gateway</h1>
    </head>
    <body>
    <form action="/login" style="top:60px;left:8px;position:fixed;">
      <h3>Username:</h3><br>
      <input type="text" style="top:40px;position:absolute;" name="userid"/><br>
      <h3>Password:</h3><br>
      <input type="password" style="top:110px;position:absolute;" name="passwd"/><br>
      <input type="submit" class="buttonStyle" style="top:150px;position:absolute;" value="Login"/>
      <input type="reset" class="buttonStyle" style="top:150px;left:110px;position:absolute;" value="Reset"/>
     </form>
    </body>
</html>
)"
