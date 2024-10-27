# distributed-chess

<project-name> is a project dedicated to recycling used IOT devices and harvesting theur MCUs to run a distributed chess bot.


# Navigation
Below is the file structure
```
.
├── frontend
│   ├── node_modules
│   │   └── typescript
│   ├── static
│   │   ├── js
│   │   └── ts
│   └── templates
└── server
    ├── handlers
    └── main.go
```

- `frontend` is quite obvious
    - `static` is where `.ts` files will be stored (although this may change)
    - `templates` contains all the `.html` files
- `server` is the backend. Despite the lack of an apt name, it will contain many of the microservices used to distribute work
    - `handlers` stores the API functions from the frontendc
    - `main.go` is the entrypoint for the web server. 