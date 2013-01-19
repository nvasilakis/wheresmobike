require 'searcher'

class SearchController < ApplicationController
  def search
    @picture = params[:picture]
    @description = params[:description]
    @date = Date.strptime(params[:date], '%m/%d/%Y')

    @results = Searcher.search

    respond_to do |format|
      format.html
      format.js
    end
  end
end
