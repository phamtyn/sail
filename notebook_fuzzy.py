
# coding: utf-8

# In[20]:


import json, gzip, os
from datetime import datetime
import re
import requests

wml_credentials = {
    "apikey"      : "",
    "instance_id" : "",
}

# Get an IAM token from IBM Cloud
url     = "https://iam.bluemix.net/oidc/token"
headers = { "Content-Type" : "application/x-www-form-urlencoded" }
data    = "apikey=" + wml_credentials["apikey"] + "&grant_type=urn:ibm:params:oauth:grant-type:apikey"
IBM_cloud_iam_uid = "bx"
IBM_cloud_iam_pwd = "bx"
response  = requests.post( url, headers=headers, data=data, auth=( IBM_cloud_iam_uid, IBM_cloud_iam_pwd ) )

if 200 != response.status_code:
    print( response.status_code )
    print( response.reason )
else:
    iam_token = response.json()["access_token"]
    
    # Send data to deployment for processing
    headers = { "Content-Type"   : "application/json", 
                "Authorization"  : "Bearer " + iam_token,
                "ML-Instance-ID" : wml_credentials["instance_id"] }
# Please insert your function deployment endpoint url
function_endpoint_url = "<your_function_deployment_endpoint_url>"


# In[21]:


from subprocess import run, PIPE, STDOUT, CalledProcessError

year_1970 = datetime(1970, 1, 1)

def download_weather():
    # Please insert your weather url including your api key
    !curl -X GET "<your_url>" \
    -H  "accept: application/json" -H  "Accept-Encoding: gzip" -o weather.gz

def check_weather2update():
    try:
        with gzip.open("weather.gz", "rt") as f:
            d = json.load(f)
    except Exception as e:
        print(repr(e))
        return
    
    now = int((datetime.utcnow() - year_1970).total_seconds())
    expire_time = d["metadata"]["expire_time_gmt"]
    if now > expire_time:
        download_weather()


def get_7day_weather():
    d = None
    with gzip.open("weather.gz", "rt") as f:
        d = json.load(f)
    assert d is not None
    
    li = []
    for i in range(1, 7):
        wea = analyze_weather(d["forecasts"][i])
        if not wea:
            return ''
        li.append(wea)
    
    for wea in li:
        if wea == 'Danger':
            return wea
    for wea in li:
        if wea == 'Bad':
            return wea
    for wea in li:
        if wea == 'Normal':
            return wea
    return 'Good'

def analyze_weather(forecast):
    
    max_temp = forecast['max_temp']
    min_temp = forecast['min_temp']
    storm = forecast['stormcon']
    wind = forecast['night']['wind_phrase']
    thunder = forecast['night']['thunder_enum_phrase']
    fog = forecast['night']['phrase_12char']
    
    m = re.search('([0-9]{1,3})\s+to\s+([0-9]{1,3})', wind)
    if m is not None:
        wind = (float(m.group(1)) + float(m.group(2))) / 2
    else:
        wind = 10
    
    if thunder == 'No thunder':
        thunder = 0
    else:
        thunder = 1
        
    m = re.search('[Ff]og', fog)
    if m is None:
        fog = 0
    else:
        fog = 1
    
    if storm is None:
        storm = 0
    
    arg = " ".join([str(max_temp), str(min_temp), str(storm),
                    str(wind), str(thunder), str(fog)])

    try:
        out = run(["./fuzzy", "-r", arg, "wea_fuzzy.c"], stdout=PIPE, stderr=STDOUT, check=True)
        return out.stdout.decode()[:-1]
        
    except CalledProcessError as error:
        print (error.stdout.decode()[:-1])
        return ''
    

if not os.path.isfile( "weather.gz" ):
    download_weather()
if not os.path.isfile( "fuzzy" ):
    ! wget https://github.com/phamtyn/sail/raw/master/fuzzy.gz
    ! gunzip fuzzy.gz
    ! chmod +x fuzzy
if not os.path.isfile( "wea_fuzzy.c" ):
    get_ipython().system(u' wget https://raw.githubusercontent.com/phamtyn/sail/master/wea_fuzzy.c')
    


# In[22]:


check_weather2update()
weather = get_7day_weather()

#print(weather)

#            FLAG	    TYPE OF SHIP	 DWT	BUILD	CITIZEN	    CARGO	        AREA	       SEASON	WEATHER	 PERIOD	   B.O.L	PSC

values = [ 
            'Panama', 'General Cargo', 12000,  2011, 'Philippines', 'Rice in bags', 'Indonesia Sea', 'Summer', weather,   8,    'Normal', 'Normal'
]
payload = { "values" : values }
response = requests.post( function_endpoint_url, json=payload, headers=headers )
print ( response.text )

