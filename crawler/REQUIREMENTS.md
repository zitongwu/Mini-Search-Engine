---
layout: page
title: Crawler Requirements Spec
type: Lecture 
date: 2021-02-01a
---

# Crawler
<br > 

## REQUIREMENTS SPEC
### Provided
<br > 
<br > 

The crawler is a standalone program that crawls the web and retrieves webpages starting from a "seed" URL.
It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given "depth".

The crawler **shall**:

 1. execute from a command line with usage syntax
    * `./crawler seedURL pageDirectory maxDepth`
    * where `seedURL` is used as the initial URL,
    * where `pageDirectory` is the directory in which to write downloaded webpages, and
    * where `maxDepth` represents the maximum crawl depth.

 2. validate it received exactly three command-line arguments and that 
    * `seedURL` must be an "internal" URL, as defined below,
    * `pageDirectory` must be the pathname for an *existing* directory, and
    * `maxDepth` must be an integer in the range [0..10].

 3. crawl all pages reachable from `seedURL`, following  links to a maximum depth of `maxDepth`; where `maxDepth=0` means that crawler only explores the page at `seedURL`, and `maxDepth=1` means that crawler only explores the page at `seedURL` and those pages to which `seedURL` links, and so forth inductively.  It shall pause at least one second between page fetches.

 4. print useful logging information as it progresses, as in the [lecture example]({{site.lectures}}/crawler).

 5. ignore URLs that are not "internal", as defined below.

 6. write each explored page to the `pageDirectory` with a unique document ID, wherein
   * the document `id` starts at 1 and increments by 1 for each new page,
   * and the filename is of form `pageDirectory/id`,
   * and the first line of the file is the URL,
   * and the second line of the file is the depth,
   * and the rest of the file is the page content (the HTML, unchanged).

The crawler **may**:

* exit with an error message and non-zero exit status if it encounters an unrecoverable error, including
	* out of memory,
	* invalid command-line arguments,
	* unable to create or write to a file of form `pageDirectory/id`,

The crawler makes the following **Assumptions**:

* The `pageDirectory` does not contain any files whose name is an integer (i.e., `1`, `2`, ...).

The crawler depends on the following **Definitions**:

* **Internal URL:** Only those beginning `http://cs50tse.cs.dartmouth.edu/tse/`.

#### Be polite

Webservers do not like crawlers (think about why).
Indeed, it you hit a web server too hard, its operator may block your crawler based on its Internet address.
Actually, they'll usually block your whole domain.
A hyperactive crawler could cause some websites to block the whole of `dartmouth.edu`.

To be polite, our crawler purposely slows its behavior by introducing a delay, sleeping for one second between fetches.

Furthermore, our crawler will limit its crawl to a specific web [server](http://cs50tse.cs.dartmouth.edu/tse/) inside CS, so we don't bother any other servers on campus or beyond.
