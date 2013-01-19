require 'net/http'
require 'nokogiri'

module Searcher

  # TODO: Move to config?
  SEARCH_URL = 'http://ec2-50-19-164-82.compute-1.amazonaws.com:8080/solr/core0/select'

  def self.search(options={})
    solr_uri = URI(SEARCH_URL)
    solr_params = {
      :q => options[:description],
      :rows => 20
    }
    solr_uri.query = URI.encode_www_form(solr_params)

    res = Net::HTTP.get_response(solr_uri)

    xml = Nokogiri::XML.parse(res.body)

    solr_results = xml.css('result doc').map do |result|
      {
        :title => result.at_css('str[name=body]').content,
        :body => result.at_css('str[name=body]').content,
        :picture => result.at_css('str[name=imageURL]').content,
        :url => 'http://craigslist.com'
      }
    end

    solr_results

  end

end
