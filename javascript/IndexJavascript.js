function runSeer() {
    $.ajax({
        url: 'http://localhost:5000/run-script',
        type: 'POST',
        success: function(response) {
            console.log(response.message);
        },
        error: function(error) {
            console.log(error);
        }
    });
}

