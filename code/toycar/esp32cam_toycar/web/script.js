const videoStream = document.getElementById('videoStream');
const refreshRate = 1000; // Refresh rate in milliseconds

function updateImage() {
  videoStream.src = `http://your_esp32cam_ip/capture?t=${new Date().getTime()}`;
}

setInterval(updateImage, refreshRate);