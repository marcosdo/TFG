const videoStream = document.getElementById('videoStream');
const refreshRate = 100; 

function updateImage() {
  videoStream.src = `http://your_esp32cam_ip/capture?t=${new Date().getTime()}`;
}

videoStream.onerror = function() {
  setTimeout(updateImage, refreshRate);
};

setInterval(updateImage, refreshRate);