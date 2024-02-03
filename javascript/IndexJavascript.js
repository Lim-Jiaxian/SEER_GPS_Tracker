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

window.onload = function() {
    document.querySelector('.dropdown-btn').addEventListener('click', function() {
        var navLinks = document.querySelector('.nav-links');
        if (navLinks.style.display === "none") {
            navLinks.style.display = "flex";
        } else {
            navLinks.style.display = "none";
        }
    });
};

function toggleMenu() {
    document.querySelector('.dropdown-btn').addEventListener('click', function() {
        var navLinks = document.querySelector('.nav-links');
        if (navLinks.style.display === "none") {
            navLinks.style.display = "flex";
        } else {
            navLinks.style.display = "none";
        }
    });
}

/* Toggle between adding and removing the "responsive" class to topnav when the user clicks on the icon */
function myFunction() {
    var x = document.getElementById("nav-links");
    if (x.className === "nav-links") {
      x.className += " responsive";
    } else {
      x.className = "nav-links";
    }
  }