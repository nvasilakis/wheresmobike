# WhereIsMoBike

A [web app] (http://where.is.mobi.ke) that aims to aid in finding a user's bike in a
case of a theft. It aggregates ads from various sites (a la craigslist) to try to
match someone who tries to sell your bike. It employs machine learning to extract features from the
bike that are not searchable solely by text search.

[where.is.mobi.ke](http://where.is.mobi.ke)

# Directories

- `web/`: web interface on Rails
- `crawler/`: Python crawler that pushes to Solr via POST
- `vision/`: C++ backend for feature extraction.
- `train/`: train data
