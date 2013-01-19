class WelcomeController < ApplicationController

  def index
    @current_tab = :home

    respond_to do |format|
      format.html
    end
  end

end
