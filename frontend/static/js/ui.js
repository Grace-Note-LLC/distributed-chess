export function updateUI(message) {
    const apiResponse = document.getElementById("apiResponse");
    if (apiResponse) {
        apiResponse.innerText = message;
    }
}
