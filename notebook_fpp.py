import json, gzip, os
from datetime import datetime
import re
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
    ! wget https://raw.githubusercontent.com/phamtyn/sail/master/wea_fuzzy.c
if not os.path.isfile( "fpp" ):
    ! wget https://github.com/phamtyn/sail/raw/master/fpp.gz
    ! gunzip fpp.gz
    ! chmod +x fpp
if not os.path.isfile( "fppdata.txt" ):
    ! wget https://raw.githubusercontent.com/phamtyn/sail/master/fppdata.txt
if not os.path.isfile( "weights.txt" ):
    ! wget https://raw.githubusercontent.com/phamtyn/sail/master/weights.txt


check_weather2update()
weather = get_7day_weather()

inputs = [ 
            'Panama', 'General Cargo', str(12000),  str(2011), 'Philippines', 'Rice in bags', 'Indonesia Sea', 'Summer', weather,   str(8),    'Normal', 'Normal'
]

arg = ",".join(inputs)

try:
    out = run(["./fpp", "-r", arg], stdout=PIPE, stderr=STDOUT, check=True)
    print(out.stdout.decode())
    
except CalledProcessError as error:
    print (error.stdout.decode())
