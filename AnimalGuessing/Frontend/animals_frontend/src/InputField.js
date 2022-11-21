import React from "react";

export default class InputField extends React.Component {

    constructor(props) {
        super(props);
        this.state = {value: ''};
        this.frontendApi = props.frontendApi;
        this.label = props.label;
        this.operation = props.operation;

        this.handleChange = this.handleChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
      }
    
      handleChange(event) {
        this.setState({value: event.target.value});
      }
    
      handleSubmit(event) {
        let text = this.state.value.split(' ').join('_')

        //alert('Submitting ' + this.operation + ': ' + text);
        
        this.props.frontendApi.sendStr(this.operation, text, (expectation, response) => {
          console.log('[' + expectation + ']: "' + response + '"')
          alert(response);
        });

        event.preventDefault();
      }
    
      render() {
        return (
          <form onSubmit={this.handleSubmit}>
            <label>
              {this.label + ' '}
              <input type="text" value={this.state.value} onChange={this.handleChange} />
            </label>
            <input type="submit" value="Submit" />
          </form>
        );
      }
  }