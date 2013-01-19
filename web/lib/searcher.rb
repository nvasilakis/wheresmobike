require 'net/http'
require 'nokogiri'

module Searcher

  def self.solr_url(url)
    @solr_url = url
  end

  def self.search(options={})
    solr_uri = URI(@solr_url)
    solr_params = {
      :q => options[:description] + " AND " +
            "date:[#{options[:date].utc.iso8601} TO NOW]",
      :rows => 20,
      :indent => 'on'
    }
    solr_uri.query = URI.encode_www_form(solr_params)
    res = Net::HTTP.get_response(solr_uri)
    xml = Nokogiri::XML.parse(res.body)

    puts solr_uri
    puts res

    solr_results = xml.css('result doc').map do |result|
      title = result.at_css('str[name=title]')
      body = result.at_css('str[name=body]')
      picture = result.at_css('arr[name=imageURL] str')
      url = result.at_css('str[name=url]')

      {
        :title => title ? title.content : '',
        :body => body ? body.content : '',
        :picture => picture ? picture.content : '',
        :url => url ? url.content : nil
      }
    end

    solr_results

  end

end
