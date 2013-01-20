require 'net/http'
require 'nokogiri'

module Searcher

  def self.solr_url(url)
    @solr_url = url
  end

  def self.inpipe_name(name)
    @inpipe_name = name
  end

  def self.outpipe_name(name)
    @outpipe_name = name
  end

  def self.comparer_command(command)
    @comparer_command = command
  end

  def self.start
    `mkfifo #{@inpipe_name}` unless File.exists? @inpipe_name
    `mkfifo #{@outpipe_name}` unless File.exists? @outpipe_name
    @inpipe = File.open(@inpipe_name, 'r+')
    @outpipe = File.open(@outpipe_name, 'w+')

    pid = spawn(@comparer_command, :in => @outpipe_name, :out => @inpipe_name)
    puts "Started KNN pid=#{pid}"
  end

  def self.search_image(paths)
    res_file = "/tmp/WheresMoBikeSearch-#{SecureRandom.hex(16)}"
    @outpipe.puts "#{paths.size} #{res_file} #{paths.join ' '}"
    @outpipe.flush
    code = @inpipe.gets

    case code
    when 'ACK'
      file = File.open(res_file)
      xml = Nokogiri::XML(file)
      file.close
      xml
    else
      puts "Image search failed: #{code}"
      nil
    end
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
        :picture => picture ? picture.content : nil,
        :url => url ? url.content : nil
      }
    end

    solr_results

  end

end
