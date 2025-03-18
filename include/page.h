const char htmlPage1[] PROGMEM = R"rawliteral(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Socket Power Monitor</title>
        <style>
            body {
                font-family: sans-serif;
                display: flex;
                justify-content: center;
                align-items: center;
                height: 100vh;
                margin: 0;
                background-color: #f4f4f4;
            }
    
            .container {
                display: flex;
                flex-direction: column;
                gap: 20px;
            }
    
            .socket {
                border: 1px solid #ccc;
                padding: 20px;
                border-radius: 8px;
                background-color: white;
                box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
                width: 250px;
            }
    
            .socket h2 {
                margin-top: 0;
                text-align: center;
            }
    
            .data-item {
                margin-bottom: 10px;
            }
    
            .data-item strong {
                display: inline-block;
                width: 120px;
            }
            .settings-button {
                display: block;
                margin-top: 5px;
                padding: 10px 20px;
                background-color: #007bff;
                color: white;
                border: none;
                border-radius: 4px;
                cursor: pointer;
                width: 100%;
                box-sizing: border-box;
            }
            .a-settings-button, .a-settings-button:link, .a-settings-button:visited, .a-settings-button:hover, .a-settings-button:active {
              text-decoration: none;
            }
        </style>
    </head>
    <body>
    
        <div class="container">
            <div class="socket" id="socket1">
                <h2>Socket 1</h2>
                <div class="data-item"><strong>Voltage:</strong> <span id="voltage1">120 V</span></div>
                <div class="data-item"><strong>Current:</strong> <span id="current1">1.5 A</span></div>
                <div class="data-item"><strong>Power:</strong> <span id="power1">180 W</span></div>
                <div class="data-item"><strong>Energy:</strong> <span id="energy1">2 kWh</span></div>
                <div class="data-item"><strong>Temperature:</strong> <span id="temperature1">35 °</span></div>
            </div>
    
            <div class="socket" id="socket2">
                <h2>Socket 2</h2>
                <div class="data-item"><strong>Voltage:</strong> <span id="voltage2">240 V</span></div>
                <div class="data-item"><strong>Current:</strong> <span id="current2">0.8 A</span></div>
                <div class="data-item"><strong>Power:</strong> <span id="power2">192 W</span></div>
                <div class="data-item"><strong>Energy:</strong> <span id="energy2">1.5 kWh</span></div>
                <div class="data-item"><strong>Temperature:</strong> <span id="temperature2">35 °</span></div>
                
            </div>
            <a class="a-settings-button" href="/settings"><button class="settings-button"><strong>SETTINGS</strong></button></a>
        </div>
    
        <script>
            function requestData() {
                fetch('/data')
                    .then(response => response.json()) // Parse response as JSON
                    .then(data => {
                        // Populate data to the data-item
                        document.getElementById('voltage1').textContent = data.voltage1 + " V";
                        document.getElementById('current1').textContent = data.current1 + " A";
                        document.getElementById('power1').textContent = data.power1 + " W";
                        document.getElementById('energy1').textContent = data.energy1 + " kWh";
                        document.getElementById('temperature1').textContent = data.temperature1 + " °C";
    
                        document.getElementById('voltage2').textContent = data.voltage2 + " V";
                        document.getElementById('current2').textContent = data.current2 + " A";
                        document.getElementById('power2').textContent = data.power2 + " W";
                        document.getElementById('energy2').textContent = data.energy2 + " kWh";
                        document.getElementById('temperature2').textContent = data.temperature2 + " °C";
                    })
                    .catch(error => console.error("Error fetching data:", error));
            }
            document.addEventListener("DOMContentLoaded", function() {
                requestData(); //initial data load.
                setInterval(requestData, 3000);
            });
        </script>
    </body>
    </html>
    )rawliteral";
    
    const char htmlPage2[] PROGMEM = R"rawliteral(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Socket Settings</title>
        <style>
        body {
            font-family: sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background-color: #f4f4f4;
        }
    
        .container {
            display: flex;
            flex-direction: column;
            gap: 20px;
        }
    
        .socket {
            border: 1px solid #ccc;
            padding: 20px;
            border-radius: 8px;
            background-color: white;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
            width: 250px;
        }
    
        .socket h2 {
            margin-top: 0;
            text-align: center;
        }
    
        .data-item {
            margin-bottom: 10px;
            display: flex;
            align-items: center;
        }
    
        .data-item strong {
            display: inline-block;
            width: 80px;
        }
    
        .data-item input[type="number"] {
            width: 60px;
            padding: 5px;
            border: 1px solid #ccc;
            border-radius: 4px;
        }
    
        .save-button {
            display: block;
            margin-top: 15px;
            padding: 10px 20px;
            background-color: #007bff;
            color: white;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            width: 100%;
            box-sizing: border-box;
        }
    
        .monitor-button {
            display: block;
            margin-top: 15px;
            padding: 10px 20px;
            background-color: #007bff;
            color: white;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            width: 100%;
            box-sizing: border-box;
        }
    
        /* Corrected CSS to remove underline from the <a> tag */
        .socket a, .socket a:link, .socket a:visited, .socket a:hover, .socket a:active {
            text-decoration: none;
        }
    </style>
    </head>
    <body>
    
        <div class="container">
            <div class="socket" id="socket1">
                <form action="/settings1" method="get">
                    <h2>Socket 1 Settings</h2>
                    <div class="data-item"><strong>Voltage:</strong> <input type="number" id="voltage1" name="voltage" min="1" max="255" value="@VOLTAGE1"></div>
                    <div class="data-item"><strong>Current:</strong> <input type="number" id="current1" name="current" min="1" max="255" value="@CURRENT1"></div>
                    <div class="data-item"><strong>Power:</strong> <input type="number" id="power1" name="power" min="1" max="255" value="@POWER1"></div>
                    <div class="data-item"><strong>Energy:</strong> <input type="number" id="energy1" name="energy" min="1" max="255" value="@ENERGY1"></div>
                    <div class="data-item"><strong>Temperature:</strong> <input type="number" id="temperature1" name="temperature" min="1" max="255" value="@TEMPERATURE1"></div>
                    <button type="submit" class="save-button">SAVE</button>
                </form>
            </div>
            <div class="socket" id="socket2">
                <h2>Socket 2 Settings</h2>
                <form action="/settings2" method="get">
                    <div class="data-item"><strong>Voltage:</strong> <input type="number" id="voltage2" name="voltage" min="1" max="255" value="@VOLTAGE2"></div>
                    <div class="data-item"><strong>Current:</strong> <input type="number" id="current2" name="current" min="1" max="255" value="@CURRENT2"></div>
                    <div class="data-item"><strong>Power:</strong> <input type="number" id="power2" name="power" min="1" max="255" value="@POWER2"></div>
                    <div class="data-item"><strong>Energy:</strong> <input type="number" id="energy2" name="energy" min="1" max="255" value="@ENERGY2"></div>
                    <div class="data-item"><strong>Temperature:</strong> <input type="number" id="temperature2" name="temperature" min="1" max="255" value="@TEMPERATURE2"></div>
                    <button type="submit" class="save-button">SAVE</button>
                </form>
                <a href="/"><button class="monitor-button">MONITOR</button></a>
            </div>
        </div>
    
        <script>
            function saveSettings(socketId) {
                // ... (Your existing JavaScript code) ...
            }
        </script>
    </body>
    </html>
    )rawliteral";