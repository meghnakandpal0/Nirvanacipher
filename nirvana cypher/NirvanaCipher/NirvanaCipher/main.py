from flask import Flask, render_template, request, redirect, url_for, flash, send_from_directory, session
from werkzeug.utils import secure_filename  
import os
import subprocess

app = Flask(__name__)
app.secret_key = 'supersecretkey'
UPLOAD_FOLDER = 'uploads'
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        if username == 'admin' and password == 'crazyshit':
            session['username'] = username
            print(f"Login successful for user {username}")
            print(f"Session after login: {session}")
            return redirect(url_for('index'))
        else:
            flash('Invalid credentials')
            return redirect(url_for('login'))
    return render_template('login.html')

@app.route('/', methods=['GET', 'POST'])
def index():
    print(f"Session before accessing index: {session}")
    if 'username' not in session:
        print("No session found. Redirecting to login.")
        return redirect(url_for('login'))

    print(f"Session found: {session['username']}")

    if request.method == 'POST':
        file = request.files['file']
        key = request.form['key']
        if file:
            filename = secure_filename(file.filename)  # Secure the filename
            filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
            file.save(filepath)
            subprocess.run(['./encryption', 'encrypt', filepath, key])
            flash('File uploaded and encrypted successfully')
            return redirect(url_for('index'))
    files = os.listdir(app.config['UPLOAD_FOLDER'])
    return render_template('index.html', files=files)

@app.route('/delete/<filename>', methods=['POST'])
def delete_file(filename):
    filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
    if os.path.exists(filepath):
        os.remove(filepath)
        flash('File deleted successfully')
    return redirect(url_for('index'))

@app.route('/share/<filename>', methods=['GET'])
def share_file(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'], filename)

@app.route('/logout')
def logout():
    session.pop('username', None)
    return redirect(url_for('login'))

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=8080)
