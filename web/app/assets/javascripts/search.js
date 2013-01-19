var WheresMoBike = {

    loadingAnimationComplete: false,

    showLoadingAnimation: function () {
        var form = $(this),
            loadingBox = $('#loading-search');

        form.fadeOut(400, function () {
            loadingBox.fadeIn(400, function () {
                WheresMoBike.loadingAnimationComplete = true;
            });
        });
    },

    hideLoadingAnimation: function () {
        var form = $(this),
            loadingBox = $('#loading-search');
        if (WheresMoBike.loadingAnimationComplete) {
            loadingBox.fadeOut();
            WheresMoBike.loadingAnimationComplete = false;
        } else {
            form.stop();
            form.hide();
            loadingBox.stop();
            loadingBox.hide();
        }
    },

    addSearchResults: function (results) {
        console.log('Added results!');
    }

};

(function($) {

    var loadingAnimationComplete = false;

    $(document)
        .delegate('#search-form', 'ajax:before',
                  WheresMoBike.showLoadingAnimation)
        .delegate('#search-form', 'ajax:complete',
                  WheresMoBike.hideLoadingAnimation)
        .delegate('#search-form', 'ajax:success', function () {
            console.log('success');
        });

})(jQuery);
