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
        thunder = False
    else:
        thunder = True
        
    m = re.search('[Ff]og', fog)
    if m is None:
        fog = False
    else:
        fog = True
    
    if storm is not None:
        if storm > 5:
            return 'Danger'
        elif storm > 0:
            storm = 'Light'
        else: storm = None
    
    if max_temp > 45:
        max_temp = 'Danger'
    elif max_temp > 35:
        max_temp = 'High'
    elif max_temp > 30:
        max_temp = 'Normal'
    else: max_temp = 'Good'
    
    if min_temp < -10:
        min_temp = 'Danger'
    elif min_temp < 5:
        min_temp = 'Low'
    elif min_temp < 15:
        min_temp = 'Normal'
    else: min_temp = 'Good'
    
    if wind > 94:
        wind = 'Gale'
    elif wind > 55:
        wind = 'Strong'
    elif wind > 40:
        wind = 'Moderate'
    elif wind > 10:
        wind = 'Light'
    else: wind = 'Calm'
    
    input_values = {
        'STORM': storm,
        'WIND': wind,
        'MAX_TEMP': max_temp,
        'MIN_TEMP': min_temp,
        'THUNDER': thunder,
        'FOG': fog,
    }
    
    def lookup(schema, input_values):
        label = schema['label']
        if label is None:
            return schema['value']
        else:
            input_value = input_values[label]
            child = schema['values'][input_value]
            return lookup(child, input_values)
    
    schema = {
        'label': 'WIND', 'values': {
            'Calm': {
                'label': 'MIN_TEMP', 'values': {
                    'Normal': {
                        'label': 'MAX_TEMP', 'values': {
                            'Normal': {
                                'label': 'FOG', 'values': {
                                    False: {
                                        'label': None, 'value': 'Normal'
                                        },
                                    True: {
                                        'label': None, 'value': 'Bad'
                                        },
                                }
                            },
                            'Danger': {
                                'label': None, 'value': 'Bad'
                            },
                            'High': {
                                'label': 'FOG', 'values': {
                                    False: { 'label': None, 'value': 'Normal' },
                                    True: { 'label': None, 'value': 'Bad' },
                                }
                            },
                            'Good': {
                                'label': 'FOG', 'values': {
                                    False: { 'label': None, 'value': 'Good' },
                                    True: { 'label': None, 'value': 'Bad' },
                                }
                            },
                        }
                    },
                    'Danger': {
                        'label': None, 'value': 'Danger'
                    },
                    'Good': {
                        'label': 'MAX_TEMP', 'values': {
                            'Normal': {
                                'label': 'FOG', 'values': {
                                    False: {
                                        'label': None, 'value': 'Good'
                                        },
                                    True: {
                                        'label': None, 'value': 'Bad'
                                        },
                                }
                            },
                            'Danger': {
                                'label': None, 'value': 'Bad'
                            },
                            'High': {
                                'label': 'FOG', 'values': {
                                    False: { 'label': None, 'value': 'Normal' },
                                    True: { 'label': None, 'value': 'Bad' },
                                }
                            },
                            'Good': {
                                'label': 'FOG', 'values': {
                                    False: { 'label': None, 'value': 'Good' },
                                    True: { 'label': None, 'value': 'Bad' },
                                }
                            },
                        }
                    },
                    'Low': {
                        'label': 'MAX_TEMP', 'values': {
                            'Normal': {
                                'label': 'FOG', 'values': {
                                    False: {
                                        'label': None, 'value': 'Normal'
                                        },
                                    True: {
                                        'label': None, 'value': 'Bad'
                                        },
                                }
                            },
                            'Danger': {
                                'label': None, 'value': 'Danger'
                            },
                            'High': {
                                'label': None, 'value': 'Bad'
                            },
                            'Good': {
                                'label': 'FOG', 'values': {
                                    False: { 'label': None, 'value': 'Normal' },
                                    True: { 'label': None, 'value': 'Bad' },
                                }
                            },
                        }
                    },
                },
            },
            'Strong': {
                'label': 'STORM', 'values': {
                    None: {
                        'label': None, 'value': 'Bad'
                    },
                    'Light': {
                        'label': 'MAX_TEMP', 'values': {
                            'Normal': {
                                'label': None, 'value': 'Bad'
                            },
                            'Danger': {
                                'label': None, 'value': 'Danger'
                            },
                            'High': {
                                'label': None, 'value': 'Bad'
                            },
                            'Good': {
                                'label': None, 'value': 'Bad'
                            },
                        }
                    },
                },
            },
            'Gale': {
                'label': None, 'value': 'Danger'
            },
            'Light': {
                'label': 'FOG', 'values': {
                    False: {
                        'label': 'MIN_TEMP', 'values': {
                            'Normal': {
                                'label': 'MAX_TEMP', 'values': {
                                    'Normal': {
                                        'label': None, 'value': 'Normal'
                                    },
                                    'Danger': {
                                        'label': None, 'value': 'Bad'
                                    },
                                    'High': {
                                        'label': None, 'value': 'Normal'
                                    },
                                    'Good': {
                                        'label': None, 'value': 'Normal'
                                    },
                                }
                            },
                            'Danger': {
                                'label': None, 'value': 'Bad'
                            },
                            'Good': {
                                'label': 'MAX_TEMP', 'values': {
                                    'Normal': {
                                        'label': None, 'value': 'Good'
                                    },
                                    'Danger': {
                                        'label': None, 'value': 'Bad'
                                    },
                                    'High': {
                                        'label': None, 'value': 'Normal'
                                    },
                                    'Good': {
                                        'label': None, 'value': 'Good'
                                    },
                                }
                            },
                            'Low': {
                                'label': 'MAX_TEMP', 'values': {
                                    'Normal': {
                                        'label': None, 'value': 'Normal'
                                    },
                                    'Danger': {
                                        'label': None, 'value': 'Bad'
                                    },
                                    'High': {
                                        'label': None, 'value': 'Bad'
                                    },
                                    'Good': {
                                        'label': None, 'value': 'Normal'
                                    },
                                }
                            },
                        }
                    },
                    True: {
                        'label': None, 'value': 'Bad'
                    },
                },
            },
            'Moderate': {
                'label': 'STORM', 'values': {
                    None: {
                        'label': 'FOG', 'values': {
                            False: {
                                'label': 'MIN_TEMP', 'values': {
                                    'Normal': {
                                        'label': 'MAX_TEMP', 'values': {
                                            'Normal': {
                                                'label': None, 'value': 'Normal'
                                            },
                                            'Danger': {
                                                'label': None, 'value': 'Bad'
                                            },
                                            'High': {
                                                'label': None, 'value': 'Normal'
                                            },
                                            'Good': {
                                                'label': None, 'value': 'Normal'
                                            },
                                        }
                                    },
                                    'Danger': {
                                        'label': None, 'value': 'Bad'
                                    },
                                    'Good': {
                                        'label': 'MAX_TEMP', 'values': {
                                            'Normal': {
                                                'label': None, 'value': 'Normal'
                                            },
                                            'Danger': {
                                                'label': None, 'value': 'Bad'
                                            },
                                            'High': {
                                                'label': None, 'value': 'Normal'
                                            },
                                            'Good': {
                                                'label': None, 'value': 'Normal'
                                            },
                                        }
                                    },
                                    'Low': {
                                        'label': 'MAX_TEMP', 'values': {
                                            'Normal': {
                                                'label': None, 'value': 'Normal'
                                            },
                                            'Danger': {
                                                'label': None, 'value': 'Bad'
                                            },
                                            'High': {
                                                'label': None, 'value': 'Bad'
                                            },
                                            'Good': {
                                                'label': None, 'value': 'Normal'
                                            },
                                        }
                                    },
                                }
                            },
                            True: {
                                'label': None, 'value': 'Bad'
                            },
                        },
                    },
                    'Light': {
                        'label': None, 'value': 'Bad'
                    },
                },
            },
        }
    }
    return lookup(schema, input_values)

if not os.path.isfile( "weather.gz" ):
    download_weather()
    
check_weather2update()


weather = get_7day_weather()

#print(weather)

#            FLAG	    TYPE OF SHIP	 DWT	BUILD	CITIZEN	     CARGO	         AREA	         SEASON	   WEATHER	 PERIOD	   B.O.L	PSC

values = [ 
            'Panama', 'General Cargo', 12000,  2011, 'Philippines', 'Rice in bags', 'Indonesia Sea', 'Summer', weather,   8,     'Normal', 'Normal'
]
payload = { "values" : values }
response = requests.post( function_endpoint_url, json=payload, headers=headers )
print ( response.text )
