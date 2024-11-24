import axios from 'axios';
import { MovePayload } from '../types/types';

const API_URL = 'http://localhost:8080/api';

export const sendMove = async (move: MovePayload) => {
  try {
    const response = await axios.post(`${API_URL}/move`, move);
    return response.data;
  } catch (error) {
    console.error('Error sending move:', error);
    throw error;
  }
}; 