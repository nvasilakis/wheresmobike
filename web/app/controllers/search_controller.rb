class SearchController < ApplicationController
  def search
    @picture = params[:picture]

    @results = [
                {:title => 'Mountain Bike', :body => 'Really cool BMX'},
                {:title => 'Blue Bike', :body => 'This bike is SO Blue!'}
               ]

    respond_to do |format|
      format.html
      format.js
    end
  end
end
