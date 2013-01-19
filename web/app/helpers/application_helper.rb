module ApplicationHelper

  def navigation(name, address)
    style = if name == @current_tab
              'class="active"'
            else
              'class=""'
            end

    raw "<li #{style}><a href=\"#{address}\">#{name.to_s.capitalize}</a></li>"
  end

end
