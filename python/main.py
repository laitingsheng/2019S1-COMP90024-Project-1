import re
from collections import defaultdict as dd
from multiprocessing import Pool, cpu_count

TFPATH = "D:/comp90024-project-1/smallTwitter.json"
X_MIN = 144.7
Y_MIN = -37.65
GRID_WIDTH = 0.15
TWITTER_TEMPLATE = '''^{"id":".*,"value":{.*"geometry":{".*"coordinates":\[(.*),(.*)]}.*"doc":{.*"entities":{.*"hashtags":\[(\{.*?\})?]'''


def process_line(line):
    data_filter = re.compile(TWITTER_TEMPLATE)
    result = data_filter.match(line)

    if result is None:
        return "", {}

    x_coor, y_coor, hashtags_data = float(result.groups()[0]), float(
        result.groups()[1]), result.groups()[2]

    horizontal_grid = chr(int(ord('1') + (x_coor - X_MIN) // GRID_WIDTH))
    vertical_grid = chr(int(ord('A') + (Y_MIN - y_coor) // GRID_WIDTH))
    grid_name = vertical_grid + horizontal_grid #hardcoded grid calculation for better efficiency

    hashtag_filter = re.compile(r'"text":"(.*?)"')
    if hashtags_data is not None:
        hashtags = [ht.lower() for ht in hashtag_filter.findall(hashtags_data)]
    else:
        hashtags = []
    return grid_name, hashtags


def processTwitterFile(data):
    file, start_point, end_point = open(TFPATH, "r", encoding='UTF-8'), \
                                   data[0], \
                                   data[1]
    grid_twit_count = dd(int)
    grid_hashtag_count = dd(dict)
    file.seek(start_point)

    file.readline()
    while file.tell() < end_point:
        new_line = file.readline()
        grid_name, hashtags = process_line(new_line)
        if not grid_name:
            continue

        grid_twit_count[grid_name] += 1
        for hashtag in hashtags:
            if hashtag in grid_hashtag_count[grid_name]:
                grid_hashtag_count[grid_name][hashtag] += 1
            else:
                grid_hashtag_count[grid_name][hashtag] = 1
    return grid_twit_count, grid_hashtag_count


if __name__ == '__main__':
    file = open(TFPATH, "r", encoding='UTF-8')
    file_size = file.seek(0, 2)
    core_num = cpu_count()
    file_length_per_core = file_size // core_num

    process_list = [(file_length_per_core * i, file_length_per_core * (i + 1))
                    for i in range(core_num)]

    pool = Pool()
    results = pool.map(processTwitterFile, process_list)

    grid_twit_count = dd(int)
    grid_hashtag_count = dd(lambda: dd(int))

    for gtc_d, ghc_d in results:
        for grid in gtc_d:
            grid_twit_count[grid] += gtc_d[grid]

            if grid in ghc_d:
                for hashtag in ghc_d[grid]:
                    grid_hashtag_count[grid][hashtag] += ghc_d[grid][hashtag]

    twitter_count_order = sorted(grid_twit_count.items(), key=lambda x: x[1],
                                 reverse=True)

    print("Twitter posts number ranking:")
    for grid in twitter_count_order:
        print(grid[0] + ": " + str(grid[1]) + " posts, ")

    print("\nTop 5 hashtags in the area:")
    for grid in grid_hashtag_count:
        top5hashtags = sorted(grid_hashtag_count[grid].items(),
                              key=lambda x: x[1], reverse=True)[:5]
        print(grid + ": " + str(top5hashtags))
