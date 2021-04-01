# Miny Search Engine
### Zitong Wu, Feb. 2021

<br > 

## Description

Mini Search Engine is a command-line web-search tool. It uses [crawler]() to crawl the web starting from a "seed" URL, [indexer]() to turn the crawled webpages into a index file mapping from words to webpages, and[querier]() to answer queries entered by the user. Click on them for more information.

## Examples
Crawler:  
<img width="496" alt="Screen Shot 2021-04-01 at 5 55 42 PM" src="https://user-images.githubusercontent.com/71951467/113290639-81293800-9324-11eb-8529-aee1e1c0fd12.png">

Indexer:

Querier:

## Structure
* Three major components of a search engine: crawler, indexer, querier
* lib and common provide the data structures and utility functions

## Instructions
To build for all directories, run `make`  
To clean up all directories, run `make clean`
To test crawler and indexer and querier, run `make test` in the corresponding directory


