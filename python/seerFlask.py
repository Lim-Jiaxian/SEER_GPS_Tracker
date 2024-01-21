from flask import Flask, jsonify , send_file
from flask_cors import CORS
import subprocess

app = Flask(__name__)
CORS(app)

@app.route('/run-script', methods=['POST'])
def home():
    subprocess.run(["python", "seer.py"])
    return jsonify({'message': 'success'})
    #return send_file('images/plot.png', mimetype='image/png')

if __name__ == '__main__':
    app.run(debug=True)