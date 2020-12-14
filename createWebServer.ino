void createWebServer(int webtype)
{

  if ( webtype == 1 )
  {
    #ifdef debug
      Serial.println("Running AP website");
     #endif
      server->on("/", []()
      {
          String content;
          content = "test page";
          server->send(200, "text/html", content);
      });  
  }
  else if (webtype == 0) 
  {
     server->on("/", []()
     {
       
          String content;
          #ifdef debug
          Serial.println("test page");
          #endif
          //content ="Received:" + responseData;
          content ="test page" + String(loopCount);
          loopCount++;
          server->send(200, "text/plain", content);
      });
      server->on("/login", []()
      {
        if(checkLoginParams())
        {  
          String content;
          content = displayMain();   
          server->send(200, "text/html", content.c_str());
        }
        else
        {
          String content;
          content = loginScreen();
          server->send(200, "text/html", content.c_str());
        }
      });
      server->on("/displayMain", []()
      {
          String content;
          content = displayMain();
          server->send(200, "text/html", content.c_str());
       
      });
      server->on("/listsensors", []()
      {
          String content;
          content = displaySensorList();
          server->send(200, "text/html", content.c_str());
       
      });
      server->on("/listsensorshelp", []()
      {
          
          String content;
          content = displaySensorListHelp();
          server->send(200, "text/html", content.c_str());
        
      });
      server->on("/dataResponse", []()
      {
          String content;
          #ifdef debug
          Serial.println("dataResponse Request");
          #endif
          String response = "";
          response = processRequest();
          #ifdef debug
          Serial.println(response);
          #endif
          server->send(200, "text/plain", response);
      });
      server->on("/retSensors", []()
      {
          String content;
          #ifdef debug
          Serial.println("retSensors Request");
          #endif
          String response;
          response += sensors.listSensors(true);
          #ifdef debug
          Serial.println(response);
          #endif
          server->send(200, "text/plain", response);
      });
      server->on("/retStatus", []()
      {
          String content;
          #ifdef debug
          Serial.println("retStatus Request");
          #endif
          String response;
          response += displayStatus();
          #ifdef debug
          Serial.println(response);
          #endif
          server->send(200, "text/plain", response);
      });
      server->on("/retRoutes", []()
      {
          String content;
          #ifdef debug
          Serial.println("retRoutes Request");
          #endif
          String response;
          response += displayRoutes();
          #ifdef debug
          Serial.println(response);
          #endif
          server->send(200, "text/plain", response);
      });
  }
}
