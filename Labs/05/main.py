import sys
from multiprocessing import Pool


def getValByKey(row, col):
    global header
    result = ''
    idx = 0

    for keycol in header.split(','):
        if keycol == col:
            result = row.split(',')[idx]
            break
        idx += 1

    result = result.strip().replace('\'', '')
    return result


def mapper(process_data):
    result = dict()
    for row in process_data:
        key = getValByKey(row, 'train_id')
        if not (key in result):
            result[key] = []

        status = getValByKey(row, 'status')
        r = 0
        if "On Time" in status:
            r = 0
        else:
            r = int(status.split(" ")[0])

        result[key].append(r)
    return result


def partition(mappings_list):
    result = dict()
    for mappings in mappings_list:
        for key in mappings:
            for val in mappings[key]:
                if not (key in result):
                    result[key] = []

                result[key].append(val)

    return result


def reducer(tuple_list):
    # print(tuple_list)
    data = dict()

    routeID = tuple_list[0]
    times = tuple_list[1]

    i = 0
    ontime = 0
    for time in times:
        i += 1
        if time < 5:
            ontime += 1

    data[routeID] = ontime * 100 / i
    return data


data = dict()
filename = sys.argv[1]
processes = int(sys.argv[2])
csvlines = []
csvfile = open(filename, 'r')
lineno = -1
header = ''

for line in csvfile:
    lineno += 1
    if lineno == 0:
        header = line
    else:
        csvlines.append(line)

numlines = len(csvlines)
lines_per_process = numlines / processes
process_data_array = []

for i in range(processes):
    start = i * (numlines / processes)
    end = (i + 1) * (numlines / processes)
    process_data_array.append(csvlines[start:end])

pool = Pool(processes=processes, )
mapping = pool.map(mapper, process_data_array)
shuffled = partition(mapping)
reduced = pool.map(reducer, shuffled.items())

for i in range(len(reduced)):
    for key, value in reduced[i].items():
        if sys.argv[3] == "1":
            print("Train: {0}\t Percent on time: {1}%".format(key, value))
