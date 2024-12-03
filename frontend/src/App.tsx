import React from 'react'
import './App.css'
import Board from './components/Board';
import Abandon from './components/Abandon';
import { BrowserRouter } from 'react-router';

function App() {
  return (
    <BrowserRouter>
      <Board/>
      <Abandon/>
    </BrowserRouter>
  )

}

export default App;
