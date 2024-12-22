


import requests
from time import sleep
import os
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

            os.system("sh /home/krish/weather-station/update_website.sh")
        else:
            print(f"Failed to fetch the webpage. HTTP Status Code: {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"An error occurred: {e}")
    sleep(10)