import './App.css';

import FrontendHandler from './FrontendHandler'
import FrontendAPI from './FrontendAPI'

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <FrontendHandler frontendApi={new FrontendAPI('http://127.0.0.1:8080')}/>
      </header>
    </div>
  );
}

export default App;
