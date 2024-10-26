import { fetchMessage } from "./api.js";
import { updateUI } from "./ui.js";

document.getElementById("apiButton")?.addEventListener("click", async () => {
    const message = await fetchMessage();
    updateUI(message);
});
