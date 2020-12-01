from src.elastic_tool import ElasticTool, ElasticToolv0

elastic_tool = ElasticTool()
elastic_tool.delete_all_index()

elastic_toolv0 = ElasticToolv0()
elastic_toolv0.delete_index()