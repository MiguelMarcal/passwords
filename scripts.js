function copyPassword() {
  var password = document.getElementById("password").textContent;
  navigator.clipboard.writeText(password);
}

function generatePassword() {
  var text = document.getElementById("input").value;
  var salt = document.getElementById("inputSalt").value;
  if (isNaN(salt) || salt < 1) {
    salt = 47;
    console.log("salt became " + salt + "by default");
  }
  if (text.length < 7) {
    putInP("password < 7, try to use a string of considerable size...");
  } else {
    console.log(encryptUsingSalt(text, BigInt(salt)));
    putInP(encryptUsingSalt(text, BigInt(salt)));
  }
}

function encryptUsingSalt(text, salt) {
  let password = "";
  for (let i = 0; i < text.length; i++) {
    const j = BigInt(text.charCodeAt(i));
    if (j >= 33n && j <= 126n) {
      const newChar = String.fromCharCode(
        Number(33n + ((j + BigInt(i) + BigInt(salt)) % 94n))
      );
      password += newChar;
    } else {
      password += text[i];
    }
  }

  return password;
}

function putInP(text) {
  var elementP = document.getElementById("password");
  elementP.textContent = text;
}
