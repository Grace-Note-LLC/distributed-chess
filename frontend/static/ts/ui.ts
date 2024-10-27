export function updateUI(message: string): void {
    const apiResponse = document.getElementById("apiResponse");
    if (apiResponse) {
        apiResponse.innerText = message;
    }
}