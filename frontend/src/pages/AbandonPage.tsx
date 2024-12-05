import React from 'react';
import { useNavigate } from 'react-router';
import './AbandonPage.css';

const AbandonPage: React.FC = () => {
  const navigate = useNavigate();

  const handleReturnHome = () => {
    navigate('/');
  };

  return (
    <div className="abandon-page">
      <h1>Couldn't handle it?</h1>
      <p>That's okay, even grandmasters resign sometimes.</p>
      <button onClick={handleReturnHome}>Return Home</button>
    </div>
  );
};

export default AbandonPage;