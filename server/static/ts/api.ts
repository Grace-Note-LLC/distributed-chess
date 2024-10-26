export async function fetchMessage(): Promise<string> {
    try {
        const response = await fetch("/api/hello");
        const data = await response.json();
        return data.message;
    } catch (error) {
        console.error("Error fetching message:", error);
        return "Error fetching message.";
    }
}