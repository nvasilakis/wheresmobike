require 'searcher'

Searcher.solr_url AppConfig.solr_url
Searcher.inpipe_name AppConfig.inpipe_name
Searcher.outpipe_name AppConfig.outpipe_name
Searcher.comparer_command AppConfig.comparer_command
Searcher.start
