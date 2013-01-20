require 'searcher'

class SearchController < ApplicationController
  def search
    @picture = params[:picture]
    @description = params[:description]
    @date = Date.strptime(params[:date], '%m/%d/%Y').to_time

    @results =
      Searcher.search(:picture => @picture,
                      :description => @description,
                      :date => @date)

    # uncomment to test waiting screen
    # sleep 5

    respond_to do |format|
      format.html
      format.js
    end
  end
end
