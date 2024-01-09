from Flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def index():
    image_path = "/images/map.jpg"  # replace with your image path
    return render_template('index.html', image = image_path) # return the image to the webpage

if (__name__ == "__main__"):
    app.run(debug=True)