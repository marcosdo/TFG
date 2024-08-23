#ifndef WEBCODE_H
#define WEBCODE_H

const char index_html[] PROGMEM =
R"rawliteral(

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32-CAM</title>
  <style>
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
  </style>
</head>
<body>
  <div id="header">TFG Coche teledirigido</div>
  <div id="videoContainer">
    <img id="videoStream" src="http://192.168.1.54/capture" alt="Camera Stream">
  </div>
  <div id="controls">
    <button id="arrow-button" style="grid-area: 1 / 2;">▲</button>
    <button id="arrow-button" style="grid-area: 2 / 1;">◄</button>
    <button id="arrow-button" style="grid-area: 2 / 3;">►</button>
    <button id="arrow-button" style="grid-area: 2 / 2;">▼</button>
  </div>
  <script>
    const videoStream = document.getElementById('videoStream');
    const refreshRate = 100;

    function updateImage() {
      videoStream.src = `http://192.168.1.54/capture?t=${new Date().getTime()}`;
    }

    videoStream.onerror = function() {
      setTimeout(updateImage, refreshRate + refreshRate);
    };

    setInterval(updateImage, refreshRate);
      
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

const char script_js[] PROGMEM = 
R"rawliteral(

const videoStream = document.getElementById('videoStream');
const refreshRate = 100;

function updateImage() {
  videoStream.src = `http://your_esp32cam_ip/capture?t=${new Date().getTime()}`;
}

videoStream.onerror = function() {
  setTimeout(updateImage, refreshRate); // Reintenta la carga en caso de error
};

setInterval(updateImage, refreshRate);

)rawliteral";


#endif