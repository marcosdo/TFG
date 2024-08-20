#ifndef HTML_H
#define HTML_H

const char* html_index PROGMEM = 
R"HTML_INDEX(
  <!DOCTYPE html>
  <html lang="en">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32-CAM Control</title>
    <style>
      body {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: center;
        height: 100vh;
        margin: 0;
        font-family: Arial, sans-serif;
        background-color: #f0f0f0;
    }
    #videoContainer {
        width: 320px;
        height: 240px;
        background-color: #000;
        margin-bottom: 20px;
        display: flex;
        align-items: center;
        justify-content: center;
    }
    #videoStream {
        max-width: 100%;
        max-height: 100%;
    }
    .button-container {
        display: grid;
        grid-template-columns: repeat(3, 60px);
        grid-template-rows: repeat(3, 60px);
        gap: 10px;
    }
    .control-button {
        width: 60px;
        height: 60px;
        background-color: #007bff;
        color: white;
        font-size: 18px;
        border: none;
        border-radius: 5px;
        cursor: pointer;
    }
    .control-button:disabled {
        background-color: #ccc;
        cursor: not-allowed;
    }
    </style>
  </head>
  <body>
    <div id="videoContainer">
      <img id="videoStream" src="http://192.168.1.54/capture" alt="Camera Stream">
    </div>
    <div class="button-container">
      <button class="control-button" style="grid-area: 1 / 2;">&#8593;</button>
      <button class="control-button" style="grid-area: 2 / 1;">&#8592;</button>
      <button class="control-button" style="grid-area: 2 / 3;">&#8594;</button>
      <button class="control-button" style="grid-area: 3 / 2;">&#8595;</button>
    </div>

    <script>
      const videoStream = document.getElementById('videoStream');
      const refreshRate = 1000; // Refresh rate in milliseconds
      function updateImage() {
          videoStream.src = `http://192.168.1.54/capture?t=${new Date().getTime()}`;
      }
      setInterval(updateImage, refreshRate);
    </script>
  </body>
  </html>
)HTML_INDEX";

#endif