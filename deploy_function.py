import numpy as np
import gzip

def retrieve_file_data(filename):
    with gzip.open(filename, "rt") as f:
        s = f.read()
        s = s.split('\n')
        s.pop()
        column_titles = s.pop(0)
        return s, column_titles

def getdata(rawdata):
    data = []
    labels = []
    for line in rawdata:
        arr = line.split(',')
        labels += [ arr.pop() ]
        data += arr
    return data, labels

dict_data = {
    'US': 0,
    'Japan': 1,
    'Panama': 2,
    'Vietnam': 3,
    'General Cargo': 0,
    'Bulk Carriers': 1,
    'Container': 2,
    'Ro-Ro': 3,
    'Oil Tanker': 4,
    'Liquefied Gas': 5,
    'Chemical Carriers': 6,
    'American': 0,
    'Japanese': 1,
    'Korean': 2,
    'Chinese': 3,
    'Philippines': 4,
    'Vietnamese': 5,
    'Multi-national': 6,
    'Rice in bags': 0,
    'Soybean milk': 1,
    'Tapioca chip': 2,
    'Grain in bulk': 3,
    'Coal': 4,
    'Cement': 5,
    'Clay': 6,
    'Ore in bulk': 7,
    'Ore in jumbo bag': 8,
    'Timber': 9,
    'Logs': 10,
    'Cars': 11,
    'Equipment': 12,
    'Steel products': 13,
    'Heavy-lift': 14,
    'Containers': 15,
    'Product oil': 16,
    'Crude oil': 17,
    'Chemical': 18,
    'Gas': 19,
    'Pacific': 0,
    'Indian Ocean': 1,
    'Atlantic': 2,
    'Indonesia Sea': 3,
    'Yellow Sea': 4,
    'Mediterranean': 5,
    'Summer': 0,
    'Winter': 1,
    'Good': 0,
    'Normal': 1,
    'Bad': 2,
    'Danger': 3,
    'Safe': 0,
    'Low Risk': 1,
    'Medium Risk': 2,
    'High Risk': 3,
    'Dangerous': 4,
}


def string2float(words):
    li = []
    for k in words:
        try:
            num = float(k)
            li.append(num)
        except ValueError:
            li.append(dict_data.get(k, 0))
    return np.array(li)

def calmaxmin(input_array, num_input):
    columns = (string2float(input_array).reshape(-1, num_input)).T
    return [ max(col) for col in columns ], [ min(col) for col in columns ]

NUM_OUTPUT_CLASSES = 5
# Please place sail.csv.gz file in the current directory
rawdata, COLUMN_TITLES = retrieve_file_data('sail.csv.gz')
NUM_INPUT = len(rawdata[0].split(',')) - 1
train_data, train_labels = getdata(rawdata)
MAX_INPUT_VALUES, MIN_INPUT_VALUES = calmaxmin(train_data, NUM_INPUT)
MAX_STRING = str(MAX_INPUT_VALUES)
MIN_STRING = str(MIN_INPUT_VALUES)

wml_credentials = {
    "apikey"      : "",
    "instance_id" : "",
    "password"    : "",
    "url"         : "",
    "username"    : ""
}
model_endpoint_url = ""

ai_parms = { "wml_credentials" : wml_credentials, "model_endpoint_url" : model_endpoint_url }

def sail_deployable_function( parms=ai_parms, num_input=NUM_INPUT, max_string = MAX_STRING, min_string = MIN_STRING ):
    import numpy as np
    
    dict_data = {
        'US': 0,
        'Japan': 1,
        'Panama': 2,
        'Vietnam': 3,
        'General Cargo': 0,
        'Bulk Carriers': 1,
        'Container': 2,
        'Ro-Ro': 3,
        'Oil Tanker': 4,
        'Liquefied Gas': 5,
        'Chemical Carriers': 6,
        'American': 0,
        'Japanese': 1,
        'Korean': 2,
        'Chinese': 3,
        'Philippines': 4,
        'Vietnamese': 5,
        'Multi-national': 6,
        'Rice in bags': 0,
        'Soybean milk': 1,
        'Tapioca chip': 2,
        'Grain in bulk': 3,
        'Coal': 4,
        'Cement': 5,
        'Clay': 6,
        'Ore in bulk': 7,
        'Ore in jumbo bag': 8,
        'Timber': 9,
        'Logs': 10,
        'Cars': 11,
        'Equipment': 12,
        'Steel products': 13,
        'Heavy-lift': 14,
        'Containers': 15,
        'Product oil': 16,
        'Crude oil': 17,
        'Chemical': 18,
        'Gas': 19,
        'Pacific': 0,
        'Indian Ocean': 1,
        'Atlantic': 2,
        'Indonesia Sea': 3,
        'Yellow Sea': 4,
        'Mediterranean': 5,
        'Summer': 0,
        'Winter': 1,
        'Good': 0,
        'Normal': 1,
        'Bad': 2,
        'Danger': 3,
        'Safe': 0,
        'Low Risk': 1,
        'Medium Risk': 2,
        'High Risk': 3,
        'Dangerous': 4,
    }

    dict_result = {
        0: 'Safe',
        1: 'Low Risk',
        2: 'Medium Risk',
        3: 'High Risk',
        4: 'Dangerous',
    }

    def string2float(words):
        li = []
        for k in words:
            try:
                num = float(k)
                li.append(num)
            except ValueError:
                li.append(dict_data.get(k, 0))
        return np.array(li)
    
    def int2string(ints):
        results = []
        for k in ints:
            results.append(dict_result[k])
        return results
    
    def rate(a, max_arg, min_arg):
        max_np = np.array(max_arg)
        return (a - min_arg) / (max_np - min_arg)

    def refine_data(input_data, num_input, max_input_values, min_input_values):
        data = string2float(input_data).reshape(-1, num_input)
        data = rate(data, max_input_values, min_input_values)
        return data
    
    def recover_array_from_string(max_string, min_string):
        li_max = []
        li_min = []
        for item in max_string[1:-1].split(','):
            li_max.append(float(item))
        for item in min_string[1:-1].split(','):
            li_min.append(float(item))
        return li_max, li_min

    def score( function_payload ):

        try:
            pred_data = function_payload["values"]
            max_values, min_values = recover_array_from_string(max_string, min_string)
            pred_data = refine_data(pred_data, num_input, max_values, min_values).tolist()
            model_payload = { "values" : pred_data }
            
            from watson_machine_learning_client import WatsonMachineLearningAPIClient
            client       = WatsonMachineLearningAPIClient( parms["wml_credentials"] )
            model_result = client.deployments.score( parms["model_endpoint_url"], model_payload )
            human_result  = int2string(model_result["values"])
            return human_result

        except Exception as e:
            
            return { "Error" : repr( e ) }
    
    return score

from watson_machine_learning_client import WatsonMachineLearningAPIClient
client       = WatsonMachineLearningAPIClient( wml_credentials )

meta_data = { client.repository.FunctionMetaNames.NAME : 'Sail model function' }
function_details = client.repository.store_function( meta_props=meta_data, function=sail_deployable_function )

function_id = function_details["metadata"]["guid"]
function_deployment_details = client.deployments.create( artifact_uid=function_id, name='Sail model function deployment' )
