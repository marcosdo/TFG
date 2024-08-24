#ifndef WEBCODE_H
#define WEBCODE_H

const char index_html[] PROGMEM =
R"rawliteral(

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="icon" href="/favicon.ico" type="image/x-icon">
  <link rel="stylesheet" href="/style.css">
  <title>ESP32-CAM</title>
</head>
<body>
  <div id="header">TFG Coche teledirigido</div>
  <div id="videoContainer">
    <img id="videoStream" src="" alt="Camera Stream">
  </div>
  <div id="controls">
    <button id="arrow-button" style="grid-area: 1 / 2;">▲</button>
    <button id="arrow-button" style="grid-area: 2 / 1;">◄</button>
    <button id="arrow-button" style="grid-area: 2 / 3;">►</button>
    <button id="arrow-button" style="grid-area: 2 / 2;">▼</button>
  </div>
  <div id ="show-fps">
    <span id="fpsDisplay">FPS: N/A</span>
  </div>

  <script>
    var wsCam;
    var wsCamURL = "ws://" + window.location.hostname + "/wsCam";
    var url = null;
    
    function initWebSockCam() {
      wsCam = new WebSocket(wsCamURL);
      wsCam.binaryType = 'blob';
        wsCam.onopen    = function(event) {
          console.log(" => wsCam connection opened");
        };
        wsCam.onclose   = function(event) {
          console.log(" => wsCam connection closed");
          setTimeout(initWebSockCam, 2000);
        };
        wsCam.onmessage = function(event) {
          var blob = event.data;
          if (url) {
            URL.revokeObjectURL(url);
          }
          var imageID = document.getElementById("videoStream");
          url = URL.createObjectURL(blob);
          imageID.src = url;

          wsCam.send(JSON.stringify({ type: "ack", status: "received" }));
        };
        wsCam.onerror = function(error) {
          console.log(' => wsCam error: ' + error.message);
        };
    }

    function initWebSock() {
      initWebSockCam();
    }

    window.onload = initWebSock;
  </script>
</body>
</html>

)rawliteral";

const char style_css[] PROGMEM = 
R"rawliteral(

    body {
      margin: 0;
      padding: 0;
      background-color: #333;
      color: white;
      display: flex;
      flex-direction: column;
      height: 100vh;
      overflow: hidden;
      align-items: center;
    }

    #videoContainer {
      flex: 2;
      width: 80%;
      max-width: 960px;
      position: relative;
      background-color: #000;
      border: 5px solid white;
    }

    #videoContainer::before {
      content: "";
      display: block;
      padding-bottom: 75%; /* 4:3 aspect ratio */
    }

    #videoStream {
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      object-fit: cover;
    }

    #controls {
      flex: 1;
      width: 80%;
      max-width: 960px;
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      grid-template-rows: repeat(2, 1fr);
      gap: 10px;
      background-color: rgba(0, 0, 0, 0.7);
      padding: 10px;
      box-sizing: border-box;
    }

    #arrow-button {
      background-color: white;
      color: black;
      font-size: 20px;
      padding: 0;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      display: flex;
      align-items: center;
      justify-content: center;
      width: 100%;
      height: 100%;
    }

    #arrow-button:active {
        background-color: lightgray;
    }

    #header {
      position: absolute;
      top: 10px;
      left: 50%;
      transform: translateX(-50%);
      background-color: rgba(0, 0, 0, 0.7);
      padding: 10px;
      border-radius: 10px;
      color: white;
      font-size: 24px;
      z-index: 1;
    }

    /* Responsive Styles */
    @media (max-width: 600px) {
      #header {
        font-size: 18px;
        padding: 5px;
      }

      #arrow-button {
        font-size: 16px;
        width: 40px;
        height: 40px;
      }

      #controls {
        grid-template-columns: repeat(3, 1fr);
        grid-template-rows: repeat(2, 1fr);
      }
    }

)rawliteral";

const uint8_t favicon_ico[] PROGMEM = {
  0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x28, 0x01,
  0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x80, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xff, 0xff, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
  0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const char script_js[] PROGMEM = 
R"rawliteral(

)rawliteral";

#endif