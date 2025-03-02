import requests

url = "http://10.0.0.104:4242"
data = {
    "left headlight": {
        "pin": "22",
        "isOn": "true"
    }
}

response = requests.post(url, json=data)
print("Status Code:", response.status_code)
