require 'searcher'

Searcher.solr_url AppConfig.solr_url
Searcher.inpipe_name('/tmp/WheresMoBikeIn')
Searcher.outpipe_name('/tmp/WheresMoBikeOut')
Searcher.comparer_command('ls')
Searcher.start
