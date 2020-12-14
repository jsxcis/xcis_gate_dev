boolean checkLoginParams()
{
  if (server->args() >= 2)
  {
          Serial.print("Username = ");
          Serial.println(server->arg(0));
          Serial.print("Password = ");
          Serial.println(server->arg(1));
          if (String (server->arg(0)) == configuration.userName)
          {
            Serial.println("User Name match");
             if ( String (server->arg(1)) == configuration.password)
             {
              Serial.println("Login successful");
              loggedIn = true;
              return true;  
             }
          }
     }
     //Serial.println("Login failed");
     return false;
}
