// #include <WebHandler.h>

// std::string htmlString = R"(
// <!DOCTYPE html>
// <html>
// <head>
//     <title>ESP32 LoRa2MQTT Gateway</title>
//     <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css"> <!-- Add FontAwesome CSS -->
//     <style>
//         /* Reset default margin and padding */
//         body, h1, h2, ul, li {
//             margin: 0;
//             padding: 0;
//         }

//         /* Add background color and font styles */
//         body {
//             font-family: 'Arial', sans-serif;
//             background-color: var(--bg-color);
//             color: var(--text-color);
//             transition: background-color 0.3s, color 0.3s;
//             position: relative;
//         }

//         /* Center the page content */
//         .container {
//             text-align: center;
//             margin: 20px auto;
//             padding: 20px;
//             border-radius: 10px;
//             box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);
//             max-width: 800px;
//             position: relative; /* Add position relative for absolute positioning */
//         }

//         /* Style the header */
//         h1 {
//             margin-bottom: 20px;
//         }

//         /* Style the status indicators */
//         .status-indicators {
//             display: flex; /* Display indicators horizontally */
//             align-items: center; /* Vertically center indicators */
//             justify-content: center; /* Horizontally center indicators */
//             margin-bottom: 10px; /* Add 10px spacing between indicators */
//         }

//         .status-indicator {
//             width: 20px;
//             height: 20px;
//             border-radius: 50%;
//             margin: 0 10px; /* Add 10px spacing between indicators */
//         }

//         .mqtt-indicator {
//             background-color: var(--mqtt-color); /* Variable for MQTT status color */
//         }

//         .lora-indicator {
//             background-color: var(--lora-color); /* Variable for LoRa status color */
//         }

//         /* Style the message list */
//         .message-list {
//             list-style: none;
//             padding: 0;
//             text-align: left;
//             margin-top: 20px;
//         }

//         .message-item {
//             border: 1px solid #ccc;
//             margin: 10px 0;
//             padding: 10px;
//             background-color: var(--message-bg-color); /* Variable for message background color */
//             border-radius: 5px;
//             box-shadow: 0 2px 2px rgba(0, 0, 0, 0.1);
//         }

//         /* Add spacing between elements */
//         h2 {
//             margin-top: 20px;
//         }

//         /* Style the status text */
//         #status-text {
//             color: #333;
//         }

//         /* Style the buttons */
//         .button-container {
//             display: flex;
//             justify-content: flex-end; /* Align buttons to the right */
//             margin-top: 20px;
//         }

//         .update-fw-button,
//         .web-serial-button,
//         .reboot-button {
//             text-decoration: none;
//             padding: 8px 20px;
//             border-radius: 5px;
//             transition: background-color 0.3s;
//             margin: 0 10px;
//             width: calc(100% / 3 - 20px); /* Calculate width for equal distribution */
//         }

//         .update-fw-button {
//             background-color: #007bff; /* Blue for "Update Firmware" button */
//             color: #fff;
//         }

//         .web-serial-button {
//             background-color: #007bff; /* Blue for "Web Serial" button */
//             color: #fff;
//         }

//         .reboot-button {
//             background-color: #ff0000; /* Red for "Reboot" button */
//             color: #fff;
//         }

//         .update-fw-button:hover,
//         .web-serial-button:hover {
//             background-color: #0056b3; /* Darker Blue on hover for "Update Firmware" and "Web Serial" */
//         }

//         .reboot-button:hover {
//             background-color: #990000; /* Darker Red on hover for "Reboot" */
//         }

//         /* Dark theme */
//         :root {
//             --bg-color: #222; /* Darker background */
//             --text-color: #fff;
//             --message-bg-color: #444; /* Darker message background */
//             --mqtt-color: @mqttStatus; /* Default MQTT status color */
//             --lora-color: @loraStatus; /* Default LoRa status color */
//             --icon-color: #ccc; /* Default icon color */
//         }

//         /* Light theme */
//         body.light-theme {
//             --bg-color: #f2f2f2;
//             --text-color: #333;
//             --message-bg-color: #f9f9f9;
//             --icon-color: #333; /* Light icon color */
//         }

//         /* Theme switch */
//         .theme-switch {
//             position: absolute;
//             top: 0;
//             left: 0;
//             display: flex;
//             align-items: center;
//             margin: 20px;
//         }

//         .theme-switch input[type="checkbox"] {
//             display: none;
//         }

//         .theme-switch label {
//             display: inline-block;
//             width: 40px;
//             height: 20px;
//             background-color: #007bff; /* Blue color for the toggle */
//             border-radius: 20px;
//             position: relative;
//             cursor: pointer;
//         }

//         .theme-switch label::before {
//             content: '';
//             position: absolute;
//             top: 2px;
//             left: 2px;
//             width: 16px;
//             height: 16px;
//             background-color: #fff; /* White color for the toggle button */
//             border-radius: 50%;
//             transition: transform 0.3s;
//         }

//         .theme-switch input[type="checkbox"]:checked + label::before {
//             transform: translateX(20px); /* Move the toggle button to the right */
//         }

//         /* Icon style */
//         .icon {
//             margin: 0 5px; /* Adjust margin for icons */
//             color: var(--icon-color); /* Set icon color */
//         }

//         .wifi-icon {
//             font-size: 24px; /* Make the Wi-Fi/Ethernet icon bigger */
//         }
//     </style>
// </head>
// <body>
//     <div class="container">
//         <div class="theme-switch">
//             <i class="fas fa-sun icon"></i>
//             <input type="checkbox" id="toggle-theme" checked>
//             <label for="toggle-theme"></label>
//             <i class="fas fa-moon icon"></i>
//         </div>
//         <img src="/lora2mqtt.png" alt="LoRa2MQTT Logo" style="max-width: 200px; margin-bottom: 20px;">
//         <h1>ESP32 LoRa2MQTT Gateway</h1>
        
//         <h2>Status Indicators</h2>
//         <div class="status-indicators">
//             <div class="status-indicator mqtt-indicator" id="mqtt-indicator"></div>
//             <span>MQTT Status</span>
//             <div class="status-indicator lora-indicator" id="lora-indicator"></div>
//             <span>LoRa Status</span>
//         </div>

//         <!-- Display the current date and time -->
//         <p id="current-time" style="font-size: 18px; margin-top: 20px;"></p>

//         <!-- Add Wi-Fi/Ethernet icon -->
//         <i class="fas fa-wifi icon wifi-icon"></i>

//         <h2>Last Received Messages</h2>
//         <ul class="message-list" id="message-list">
//             <li class="message-item">
//                 <strong>Timestamp:</strong> 2023-09-24 10:00:00<br>
//                 <strong>Content:</strong> Message 1: Hello, World!
//             </li>
//             <li class="message-item">
//                 <strong>Timestamp:</strong> 2023-09-24 10:15:00<br>
//                 <strong>Content:</strong> Message 2: Temperature: 25°C
//             </li>
//             <li class="message-item">
//                 <strong>Timestamp:</strong> 2023-09-24 10:30:00<br>
//                 <strong>Content:</strong> Message 3: Sensor Data: 123
//             </li>
//         </ul>

//         <!-- Button container -->
//         <div class="button-container">
//             <a href="/update" class="update-fw-button" target="_blank">Update FW</a>
//             <a href="/webserial" class="web-serial-button" target="_blank">Web Serial</a>
//             <a href="/reboot" class="reboot-button">Reboot</a>
//         </div>
//     </div>

//     <script>
//         // Function to display the current date and time
//         function displayCurrentTime() {
//             const currentTimeElement = document.getElementById('current-time');
//             const now = new Date();
//             const formattedTime = now.toLocaleString('en-US', { month: '2-digit', day: '2-digit', year: 'numeric', hour: '2-digit', minute: '2-digit', second: '2-digit' });
//             currentTimeElement.textContent = `Current Time: ${formattedTime}`;
//         }

//         // Toggle theme
//         const toggleTheme = document.getElementById('toggle-theme');
//         toggleTheme.addEventListener('change', function() {
//             document.body.classList.toggle('light-theme');
//         });

//         // Update the current time every second
//         setInterval(displayCurrentTime, 1000);

//         // Refresh the page every minute
//         setInterval(function() {
//             location.reload();
//         }, 60000); // 60000 milliseconds = 1 minute
//     </script>
// </body>
// </html>

//     )";

// WebHandler::WebHandler()
// {
// }

// char *WebHandler::update()

// {
//     int strLength = htmlString.length() + 1;
//     char *outputChar = new char[strLength];
//     htmlString.toCharArray(outputChar, strLength);

//     return outputChar;
// }