import React from 'react'
import './App.css'
import Board from './components/Board';
import Abandon from './components/Abandon';
import StartPage from './pages/StartPage';
import AbandonPage from './pages/AbandonPage';
import { BrowserRouter, Routes, Route } from 'react-router';

function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<StartPage />} />
        <Route path="/game" element={
          <>
            <Board />
            <Abandon />
          </>
        } />
       <Route path="/abandon" element={<AbandonPage />} />

      </Routes>
    </BrowserRouter>
  )
}

export default App;
