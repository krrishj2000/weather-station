import requests
from time import sleep
import os
import re
from datetime import datetime
import pandas as pd
# URL of the webpage
url = "http://10.70.0.57/"

while 1:
    try:
            # Send GET request to the URL
            response = requests.get(url, timeout=5)
            print(response)
            # Check if the request was successful
            if response.status_code == 200:
                # Save the HTML content to a file
                with open("/home/krish/weather-station/index.html", "w", encoding="utf-8") as file:
                    file.write(response.text)
                print("HTML saved successfully as '/home/krish/weather-station/index.html'")
                html_content=response.text
                temperature_match = re.search(r'id="temperature">([\d.]+)', html_content)
                humidity_match = re.search(r'id="humidity">([\d.]+)', html_content)
        
                if temperature_match and humidity_match:
                    temperature = float(temperature_match.group(1))
                    humidity = float(humidity_match.group(1))
                    current_time_iso = datetime.now().isoformat()
                    print(f"Temperature: {temperature}°C")
                    print(f"Humidity: {humidity}%")
                    print(f"Current time: {current_time_iso}")
                    df=pd.DataFrame()
                    df['time']=current_time_iso
                    df['temp']=temperature
                    df['rh']=humidity
                    log_folder = r"log"
        
                    # Create the folder if it doesn't exist
                    if not os.path.exists(log_folder):
                        print("Logs folder not there; so creating...")
                        os.makedirs(log_folder)
                    df.to_csv('/home/krish/weather-station/log/'+current_time_iso+".csv")
                else:
                    print("Temperature or humidity data not found.")
                os.system("sh /home/krish/weather-station/update_website.sh")
            else:
                print(f"Failed to fetch the webpage. HTTP Status Code: {response.status_code}")
            sleep(60)
    except:
        print("Device not connected; no data received")
