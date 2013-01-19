$(document)
    .delegate('#search-form', 'ajax:before', function () {
        var form = $(this),
            loadingBox = $('#loading-search');
        form.fadeOut(400, function () {
            loadingBox.fadeIn(400, function () {
                console.log('finished animation');
            });
        });
    })
    .delegate('#search-form', 'ajax:complete', function () {
        var loadingBox = $('#loading-search');
        console.log('complete');
        loadingBox.fadeOut();
    })
    .delegate('#search-form', 'ajax:success', function () {
        var form = $(this),
            loadingBox = $('#loading-search');
        console.log('success');
        loadingBox.fadeOut();
    });
