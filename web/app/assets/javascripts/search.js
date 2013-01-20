var WheresMoBike = {

    loadingAnimationComplete: false,

    showLoadingAnimation: function () {
        var form = $(this),
            search = $('#search'),
            loadingBox = $('#loading-search');

        search.fadeOut(400, function () {
            loadingBox.fadeIn(400, function () {
                WheresMoBike.loadingAnimationComplete = true;
            });
        });
    },

    hideSearchForm: function () {
        var form = $(this),
            search = $('#search'),
            loadingBox = $('#loading-search');
        if (WheresMoBike.loadingAnimationComplete) {
            search.hide();
            loadingBox.hide();
            WheresMoBike.loadingAnimationComplete = false;
        } else {
            search.stop();
            search.hide();
            loadingBox.stop();
            loadingBox.hide();
        }
    },

    showSearchResults: function (response) {
        var results = response['results'],
            resultsDiv = $('#search-results'),
            resultsUl = resultsDiv.children('ul'),
            searchSummary = resultsDiv.children('.summary');

        var pictureUrl = response['search']['picture'];

        resultsUl.empty();

        if (pictureUrl) {
            searchSummary.children('.image').html(
                '<img src="' + pictureUrl + '" ' +
                     'alt="' + response['search']['description'] + '" ' +
                     'title="' + response['search']['description'] + '" />'
            );
        } else {
            searchSummary.children('.image').empty();
            searchSummary.children('.description').text(
                response['search']['description']
            );
        }

        if (results.length == 0) {
            resultsDiv.children('.no-results').show();
        } else {
            resultsDiv.children('.no-results').hide();
            results.forEach(function (result) {
                resultsUl.append(WheresMoBike.showSearchResult(result));
            });
        }

        resultsDiv.fadeIn();
    },

    showSearchResult: function (result) {
        return '<li class="row">' +
            '<div class="span2">' +
              (result.picture ?
                '<a target="_blank" href="' + result.url + '">' +
                   '<img src="' + result.picture + '" />' +
                '</a>' :
                '<a target="_blank" class="no-image" ' +
                   'href="' + result.url + '">' +
                   'No Image Available' +
                '</a>') +
            '</div>' +
            '<div class="span10">' +
              '<div class="title">' +
                '<a target="_blank" href="' + result.url + '">' + result.title + '</a>' +
              '</div>' +
              '<span class="body">' + result.body + '</span>' +
            '</div>' +
            '</li>';
    }

};

(function($) {

    $(document)
        .delegate('#search-form', 'ajax:before',
                  WheresMoBike.showLoadingAnimation)
        .delegate('#search-form', 'ajax:complete',
                  WheresMoBike.hideSearchForm)

        .delegate('#picture-select', 'click', function () {
            $('input[name=picture]').click();
        })
        .delegate('input[name=picture]', 'change', function () {
            var filename = $(this).val();
            filename = filename.replace(/^.+\\/, '');
            $('#picture-path').text(filename);
        });
    $(function () {
        $('.field label a').tooltip({
            placement: 'right',
            title: function () {
                var field = $(this).closest('.field');
                return field.children('.field-description').text();
            }
        });
          //$(document).ready(function(){
            $('#show-one').fadeIn(1000, function(){
              $('#show-two').fadeIn(1000, function(){
                $('#show-three').fadeIn(1000, function(){
                  WheresMoBike.glDemoConcept();
                });
              });
            });
          //});
        $('.field input[name=date]').datepicker();

    });

})(jQuery);
