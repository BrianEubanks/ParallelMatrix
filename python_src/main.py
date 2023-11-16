from os import walk
import statistics
import os


def main():
    # find the files to read
    curr_dir_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(curr_dir_path, '..', 'c', 'results', 'LU_PAR')
    f = []
    for (dirpath, dirnames, filenames) in walk(data_path):
        f.extend(filenames)

    # read the files and store the data
    print('Crout Comp')
    data = []
    for file_name in f:
        file_path = os.path.join(data_path, file_name)
        with open(file_path, 'r') as file:
            file_lines = file.read().splitlines()[2:-3]
            # print(file_lines)
            is_parallel_lu = bool(int(file_lines.pop(0).split(' ')[-1]))
            is_parallel_mult = bool(int(file_lines.pop(0).split(' ')[-1]))
            row_size = int(file_lines.pop(0).split(' ')[-1])
            file_lines.pop(0)
            err_line = file_lines.pop(0).split(' ')
            error_rate = (int(err_line[0]) / int(err_line[2]))
            file_lines.pop(0)
            run_data = file_lines
            data.append({
                'file_name': file_name,
                'is_parallel_lu': is_parallel_lu,
                'is_parallel_mult': is_parallel_mult,
                'row_size': row_size,
                'error_rate': error_rate,
                'run_data': run_data,
            })

    # gather error rates
    # error_rates = {}
    # for run in data:
    #     current_err_rates = error_rates.get(run['row_size'], [])
    #     current_err_rates.append(run['error_rate'])
    #     error_rates[run['row_size']] = current_err_rates
    #
    # # get averages
    # error_rates = dict(sorted(error_rates.items()))
    # for size, rates in error_rates.items():
    #     print(f'{size}: {statistics.mean(rates)}')
    # print(error_rates)

    res = {}
    for run_data in data:
        # if not run_data['is_parallel_lu'] and not run_data['is_parallel_mult']:
        lu = []
        mult = []
        moe = []
        runs = run_data['run_data']
        for run in runs:
            run = run.split(' , ')
            lu.append(float(run[1]))
            mult.append(float(run[2]))
            moe.append(float(run[3]))
        res[run_data['file_name']] = {
            'LU_TIME': statistics.mean(lu),
            'MULT_TIME': statistics.mean(mult),
            # 'LU_TIME': statistics.mean(lu),
        }
        # print(run_data['file_name'])
        # print(f'lu: {statistics.mean(lu)}')
        # print(f'mult: {statistics.mean(mult)}')
        # print(f'moe: {statistics.mean(moe)}')
    import json
    res = dict(sorted(res.items()))
    print(json.dumps(res, indent=2))



# average error rate by matrix size
    # for file in
    # print(f)


if __name__ == "__main__":
    main()
