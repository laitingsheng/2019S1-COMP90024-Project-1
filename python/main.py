from multiprocessing import cpu_count, Pool
from sys import argv
from collections import defaultdict as dd
import ijson

TFPATH = "tinyTwitter.json"
X_MIN = 144.700000
Y_MIN = -37.650000
GRID_WIDTH = 0.15
DEFAULT_CORE_NUM = 4

def process_twit(twit):
	x_coor, y_coor = twit["coordinates"]
	horizontal_grid = chr(ord('1') + (x_coor - X_MIN) // GRID_WIDTH)
	vertical_grid = chr(ord('A') + (y_coor - Y_MIN) // GRID_WIDTH)
	grid_name = horizontal_grid + vertical_grid
	hash_tags = [ht["text"] for ht in twit["doc"]["entities"]["hashtags"]]
	return grid_name, hash_tags

def processTwitterFile(data):
	file, start_point, length = data
	twits = ijson.items(file, 'rows.item')

	for i in range(start_point):
		next(twits)

	grid_twit_count = dd(int)
	grid_hashtag_count = dd(dict)
	for i in range(length):
		grid, hashtags = process_twit(next(twits))
		grid_twit_count[grid] += 1
		for hashtag in hashtags:
			if hashtag in  grid_hashtag_count[grid]:
				grid_hashtag_count[grid][hashtag] += 1
			else:
				grid_hashtag_count[grid][hashtag] = 1

	return grid_twit_count, grid_hashtag_count

if __name__ == '__main__':
	file = open(TFPATH, "r", encoding='UTF-8')
	twits = ijson.items(file, 'rows.item')
	num_of_twit = 0
	for i in twits:
		num_of_twit += 1

	process_list = [(file, num_of_twit // DEFAULT_CORE_NUM * i, 
					min(num_of_twit // DEFAULT_CORE_NUM * (i + 1), num_of_twit) - num_of_twit // DEFAULT_CORE_NUM * i) for i in range(num_of_twit)]

	pool = Pool()
	results = pool.map(processTwitterFile, process_list)
	gtc_dicts = [re[0] for re in results]
	ghc_dicts = [re[1] for re in results]
	grid_twit_count = dd(int)
	grid_hashtag_count = dd(dict)
	for gtc_d in gtc_dicts:
		for grid in gtc_d:
			grid_twit_count[grid] += gtc_d[grid]

	for ghc_d in ghc_dicts:
		for grid in ghc_d:
			for hashtag in ghc_d[grid]:
				if hashtag in grid_hashtag_count[grid]:
					grid_hashtag_count[grid][hashtag] += ghc_d[grid][hashtag]
				else:
					grid_hashtag_count[grid][hashtag] = ghc_d[grid][hashtag]

	twitter_count_order = sorted(grid_twit_count.items(), lambda x: x[1], reverse = True)
	for grid in twitter_count_order:
		print(grid[0] + ": " + str(grid[1]) + " posts, ")

	for grid in grid_hashtag_count:
		top5hashtags = sorted(grid_hashtag_count[grid].items(), lambda x: x[1], reverse = True)[:5]
		print(grid + ": " + str(top5hashtags))