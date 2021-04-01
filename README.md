# Miny Search Engine
### Zitong Wu, Feb. 2021

<br > 

## Description

Mini Search Engine is a command-line web-search tool. It uses [crawler](https://github.com/zitongwu0301/Mini-Search-Engine/tree/main/crawler) to crawl the web starting from a "seed" URL, [indexer](https://github.com/zitongwu0301/Mini-Search-Engine/tree/main/indexer) to turn the crawled webpages into a index file mapping from words to webpages, and[querier](https://github.com/zitongwu0301/Mini-Search-Engine/tree/main/querier) to answer queries entered by the user. Click on them for more information.

## Structure
* Three major components of a search engine: crawler, indexer, querier
* lib and common provide the data structures and utility functions

## Instructions
* To build for all directories, run `make`  
* To clean up all directories, run `make clean`  
* To test crawler and indexer and querier, run `make test` in the corresponding directory

## Examples
Crawler:  
<img width="524" alt="Screen Shot 2021-04-01 at 8 09 58 PM" src="https://user-images.githubusercontent.com/71951467/113291867-3e685f80-9326-11eb-9759-eb6d5c157c49.png">

Indexer:  
<img width="465" alt="Screen Shot 2021-04-01 at 6 08 28 PM" src="https://user-images.githubusercontent.com/71951467/113291681-f1848900-9325-11eb-93d7-cbe368c3956e.png"> 

Querier:  
<img width="889" alt="Screen Shot 2021-04-01 at 6 14 30 PM" src="https://user-images.githubusercontent.com/71951467/113291252-58557280-9325-11eb-84fd-c25d60773ff9.png">



